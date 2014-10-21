#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include "RetweetCollection.h"

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
   void setup() override {
      collection.add(Tweet());
   }
};

TEST(ARetweetCollectionWithOneTweet, IsNoLongerEmpty) {
   CHECK_FALSE(collection.isEmpty());
}

TEST(ARetweetCollectionWithOneTweet, HasSizeOfOne) {
   LONGS_EQUAL(1, collection.size());
}

TEST(ARetweetCollection, IgnoresDuplicateTweetAdded) {
   Tweet tweet("msg", "@user");
   Tweet duplicate(tweet);
   collection.add(tweet);
   collection.add(duplicate);
   LONGS_EQUAL(1, collection.size());
}
