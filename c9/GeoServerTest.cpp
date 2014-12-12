#include "CppUTest/TestHarness.h"
#include "CppUTestExtensions.h"

#include <condition_variable>
#include <thread>
#include <chrono>

#include "GeoServer.h"
#include "VectorUtil.h"
#include "TestTimer.h"
#include "ThreadPool.h"
#include "Work.h"

using namespace std;
using std::chrono::milliseconds;

/** to_string() is not defined in Cygwin GCC 4.8.3 */
inline string to_string(int i) {
    stringstream ss{};
    ss << i;
    return ss.str();
}

TEST_GROUP(AGeoServer) {
   
   GeoServer server;

   const string aUser{"auser"};
   const double LocationTolerance{0.005};
};

TEST(AGeoServer, AnswersUnknownLocationWhenUserNoLongerTracked) {
   server.track(aUser);
   server.stopTracking(aUser);

   // slow reading. Fix this.
   auto location = server.locationOf(aUser);
   CHECK_EQUAL(numeric_limits<double>::infinity(), location.latitude());
}

TEST(AGeoServer, TracksAUser) {
   server.track(aUser);

   CHECK_TRUE(server.isTracking(aUser));
}

TEST(AGeoServer, IsNotTrackingAUserNotTracked) {
   CHECK_FALSE(server.isTracking(aUser));
}

TEST(AGeoServer, TracksMultipleUsers) {
   server.track(aUser);
   server.track("anotheruser");

   CHECK_FALSE(server.isTracking("thirduser"));
   CHECK_TRUE(server.isTracking(aUser));
   CHECK_TRUE(server.isTracking("anotheruser"));
}

TEST(AGeoServer, IsTrackingAnswersFalseWhenUserNoLongerTracked) {
   server.track(aUser);
   
   server.stopTracking(aUser);

   CHECK_FALSE(server.isTracking(aUser));
}

TEST(AGeoServer, UpdatesLocationOfUser) {
   server.track(aUser);

   server.updateLocation(aUser, Location{38, -104});

   auto location = server.locationOf(aUser);
   DOUBLES_EQUAL(38, location.latitude(), LocationTolerance);
   DOUBLES_EQUAL(-104, location.longitude(), LocationTolerance);
}

TEST(AGeoServer, AnswersUnknownLocationForUserNotTracked) {
   CHECK_TRUE(server.locationOf("anAbUser").isUnknown());
}

TEST(AGeoServer, AnswersUnknownLocationForTrackedUserWithNoLocationUpdate) {
   server.track(aUser);

   CHECK_TRUE(server.locationOf(aUser).isUnknown());
}

TEST(AGeoServer, AnswersUnknownLocationForUserNoLongerTracked) {
   server.track(aUser);
   server.updateLocation(aUser, Location(40, 100));

   server.stopTracking(aUser);

   CHECK_TRUE(server.locationOf(aUser).isUnknown());
}

class GeoServerUsersInBoxTestFixture {
public:
   GeoServer* server;
   shared_ptr<ThreadPool> pool;
   
   GeoServerUsersInBoxTestFixture() {
      server = new GeoServer{};
      server->track(aUser);
      server->track(bUser);
      server->track(cUser);
      server->updateLocation(aUser, aUserLocation);
   }
   
   ~GeoServerUsersInBoxTestFixture() {
      delete server;
   }

   const double TenMeters { 10 };
   const double Width { 2000 + TenMeters };
   const double Height { 4000 + TenMeters};
   const string aUser { "auser" };
   const string bUser { "buser" };
   const string cUser { "cuser" };

   Location aUserLocation { 38, -103 };

   void addUsersAt(unsigned int number, const Location& location) {
      for (unsigned int i{0}; i < number; i++) {
         string user{"user" + to_string(i)};
         server->track(user);
         server->updateLocation(user, location);
      }
   }
};

TEST_GROUP(AGeoServer_UsersInBox) {
    
   GeoServerUsersInBoxTestFixture f;
   
   class GeoServerUserTrackingListener: public GeoServerListener {
   public:
      void updated(const User& user) { Users.push_back(user); }
      vector<User> Users;
   } trackingListener;

   class SingleThreadedPool: public ThreadPool {
   public:
      virtual void add(Work work) override { work.execute(); }
   };

   void setup() override {
      f.pool = make_shared<SingleThreadedPool>();
      f.server->useThreadPool(f.pool);
   }

   vector<string> UserNames(const vector<User>& users) {
      return Collect<User,string>(users, [](User each) { return each.name(); });
   }
};

TEST(AGeoServer_UsersInBox, AnswersUsersInSpecifiedRange) {
   f.pool->start(0);
   f.server->updateLocation(
      f.bUser, Location{f.aUserLocation.go(f.Width / 2 - f.TenMeters, East)}); 

   f.server->usersInBox(f.aUser, f.Width, f.Height, &trackingListener);

   CHECK_EQUAL(vector<string> { f.bUser }, UserNames(trackingListener.Users));
}

TEST(AGeoServer_UsersInBox, AnswersOnlyUsersWithinSpecifiedRange) {
   f.pool->start(0);
   f.server->updateLocation(
      f.bUser, Location{f.aUserLocation.go(f.Width / 2 + f.TenMeters, East)}); 
   f.server->updateLocation(
      f.cUser, Location{f.aUserLocation.go(f.Width / 2 - f.TenMeters, East)}); 

   f.server->usersInBox(f.aUser, f.Width, f.Height, &trackingListener);

   CHECK_EQUAL(vector<string> { f.cUser }, UserNames(trackingListener.Users));
}

TEST(AGeoServer_UsersInBox, HandlesLargeNumbersOfUsers) {
   f.pool->start(0);
   const unsigned int lots {5000};
   f.addUsersAt(lots, Location{f.aUserLocation.go(f.TenMeters, West)});

   TestTimer timer;
   f.server->usersInBox(f.aUser, f.Width, f.Height, &trackingListener);

   LONGS_EQUAL(lots, trackingListener.Users.size());
}

TEST_GROUP(AGeoServer_ScaleTests) {

   GeoServerUsersInBoxTestFixture f;
   
   class GeoServerCountingListener: public GeoServerListener {
   public:
      void updated(const User& user) override {
         (void)user; // unused
         unique_lock<std::mutex> lock(mutex_);
         Count++;
         wasExecuted_.notify_all();
      }

      void waitForCountAndFailOnTimeout(unsigned int expectedCount, 
            const milliseconds& time=milliseconds(10000)) {
         unique_lock<mutex> lock(mutex_);
         CHECK_TRUE(wasExecuted_.wait_for(lock, time,  
                 [&]{ 
                      LONGS_EQUAL(expectedCount, Count);
                      return (expectedCount == Count); 
                  }));
      }

      condition_variable wasExecuted_;
      unsigned int Count{0};
      mutex mutex_;
   };

   GeoServerCountingListener countingListener;
   shared_ptr<thread> t;

   void setup() override {
      f.pool = make_shared<ThreadPool>();
      f.server->useThreadPool(f.pool);
   }

   void teardown() override {
      t->join();
   }
};

TEST(AGeoServer_ScaleTests, HandlesLargeNumbersOfUsers) {
   f.pool->start(4);
   const unsigned int lots{5000};
   f.addUsersAt(lots, Location{f.aUserLocation.go(f.TenMeters, West)});

   t = make_shared<thread>(
         [&] { f.server->usersInBox(f.aUser, f.Width, f.Height, &countingListener); });

   countingListener.waitForCountAndFailOnTimeout(lots);
}
