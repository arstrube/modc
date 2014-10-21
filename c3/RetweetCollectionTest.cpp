#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

TEST_GROUP(ARetweetCollection) {};

TEST(ARetweetCollection, IsEmptyWhenCreated) {
   RetweetCollection retweets;
}
