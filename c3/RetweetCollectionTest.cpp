#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

class RetweetCollection {};

TEST_GROUP(ARetweetCollection) {};

TEST(ARetweetCollection, IsEmptyWhenCreated) {
   RetweetCollection retweets;
   (void)retweets;
   FAIL("To be implemented");
}
