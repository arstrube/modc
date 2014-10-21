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

TEST(ARetweetCollection, IsNoLongerEmptyAfterTweetAdded) {
   collection.add(Tweet());
   CHECK_FALSE(collection.isEmpty());
}

TEST(ARetweetCollection, HasSizeOfOneAfterTweetAdded) {
   collection.add(Tweet());
   LONGS_EQUAL(1, collection.size());
}

TEST(ARetweetCollection, IsEmptyAfterRemovingTweet) {
   collection.add(Tweet());
   collection.remove(Tweet());
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
