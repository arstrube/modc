#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include "RetweetCollection.h"
#include <memory>

using namespace std;

TEST_GROUP(ARetweetCollection) {
   RetweetCollection collection;
};

TEST(ARetweetCollection, IsEmptyWhenCreated) {
   CHECK(collection.isEmpty());
}

TEST(ARetweetCollection, HasSizeZeroWhenCreated) {
   LONGS_EQUAL(0, collection.size());
}

TEST(ARetweetCollection, IsEmptyWhenItsSizeIsZero) {
   LONGS_EQUAL(0, collection.size());
   CHECK(collection.isEmpty());
}

TEST(ARetweetCollection, IsNotEmptyWhenItsSizeIsNonZero) {
   collection.add(Tweet());
   CHECK(collection.size() > 0);
   CHECK_FALSE(collection.isEmpty());
}

TEST_GROUP(ARetweetCollectionWithOneTweet) {
   RetweetCollection collection;
   shared_ptr<Tweet> tweet;
   
   void setup() override {
      tweet = shared_ptr<Tweet>(new Tweet("msg", "@user"));
      collection.add(*tweet);
   }
};

TEST(ARetweetCollectionWithOneTweet, IsNoLongerEmpty) {
   CHECK_FALSE(collection.isEmpty());
}

TEST(ARetweetCollectionWithOneTweet, HasSizeOfOne) {
   LONGS_EQUAL(1, collection.size());
}

TEST(ARetweetCollectionWithOneTweet, IgnoresDuplicateTweetAdded) {
   Tweet duplicate(*tweet);
   collection.add(duplicate);
   LONGS_EQUAL(1, collection.size());
}
