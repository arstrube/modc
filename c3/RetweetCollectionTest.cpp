#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include "RetweetCollection.h"

TEST_GROUP(ARetweetCollection) {
   RetweetCollection collection;
};

/** CppUTest has no equivalent to MATCHER_P()
 *  Use a macro instead. Why a macro?
 *  A macro will fail _in_ the test case, wheras
 *  a helper method will fail in the helper
 *  method, making debugging more difficult.
 */
#define HAS_SIZE(arg, expected) { \
   LONGS_EQUAL(expected, arg.size()); \
   CHECK(arg.isEmpty() == (0 == expected)); \
}

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
   HAS_SIZE(collection, 0);
}
