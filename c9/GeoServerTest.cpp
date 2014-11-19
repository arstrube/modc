#include "CppUTest/TestHarness.h"
#include "CppUTestExtensions.h"

#include "GeoServer.h"
#include "VectorUtil.h"
#include "TestTimer.h"
// ...
using namespace std;

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

TEST_GROUP(AGeoServer_UsersInBox) {
   GeoServer server;

   const double TenMeters { 10 };
   const double Width { 2000 + TenMeters };
   const double Height { 4000 + TenMeters};
   const string aUser { "auser" };
   const string bUser { "buser" };
   const string cUser { "cuser" };

   Location aUserLocation { 38, -103 };

   void setup() override {
      server.track(aUser);
      server.track(bUser);
      server.track(cUser);

      server.updateLocation(aUser, aUserLocation);
   }

   vector<string> UserNames(const vector<User>& users) {
      return Collect<User,string>(users, [](User each) { return each.name(); });
   }
};

TEST(AGeoServer_UsersInBox, AnswersUsersInSpecifiedRange) {
   server.updateLocation(
      bUser, Location{aUserLocation.go(Width / 2 - TenMeters, East)}); 

   auto users = server.usersInBox(aUser, Width, Height);

   CHECK_EQUAL(vector<string> { bUser }, UserNames(users));
}

TEST(AGeoServer_UsersInBox, AnswersOnlyUsersWithinSpecifiedRange) {
   server.updateLocation(
      bUser, Location{aUserLocation.go(Width / 2 + TenMeters, East)}); 
   server.updateLocation(
      cUser, Location{aUserLocation.go(Width / 2 - TenMeters, East)}); 

   auto users = server.usersInBox(aUser, Width, Height);

   CHECK_EQUAL(vector<string> { cUser }, UserNames(users));
}

TEST_GROUP(AGeoServer_StressTest) {
   GeoServer server;
   TestTimer t0;
   const unsigned int lots {5000000};
   const string aUser { "auser" };

   const double TenMeters { 10 };
   const double Width { 2000 + TenMeters };
   const double Height { 4000 + TenMeters};

   Location aUserLocation { 38, -103 };

   void setup() override {
      TestTimer t1{"    setup()"};
      server.track(aUser);
      server.updateLocation(aUser, aUserLocation);
      for (unsigned int i{0}; i < lots; i++) {
         string user{"user" + to_string(i)};
         server.track(user);
         server.updateLocation(user, aUserLocation);
      }
   }
};

TEST(AGeoServer_StressTest, HandlesLargeNumbersOfUsers) {

   TestTimer t2("    usersInBox()");
   auto users = server.usersInBox(aUser, Width, Height);

   CHECK_EQUAL(lots, users.size());
}