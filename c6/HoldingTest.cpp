#include "cpputest/testharness.h"

#include "Holding.h"
#include "DateTestConstants.h"

using namespace boost::gregorian;

TEST_GROUP(AHolding) {
   Holding holding_;
   void setup() {
      holding_ = {};
   }
};

TEST(AHolding, IsEmptyWhenCreated) {
   LONGS_EQUAL(0, holding_.ShareCount());
}

TEST(AHolding, IsNotEmptyAfterAdd) {
   PurchaseRecord p(20, arbitraryDate);
   holding_.Add(p);
   CHECK(holding_.ShareCount() != 0);
}

TEST(AHolding, ShareCountAccumulatesAddedPurchases) {
   PurchaseRecord p1(20, arbitraryDate);
   PurchaseRecord p2(3, arbitraryDate);
   holding_.Add(p1);
   holding_.Add(p2);

   LONGS_EQUAL(20 + 3, holding_.ShareCount());
}

TEST(AHolding, ShareCountReflectsNegativePurchases) {
   PurchaseRecord p1(20, arbitraryDate);
   PurchaseRecord p2(-5, arbitraryDate);
   holding_.Add(p1);
   holding_.Add(p2);

   LONGS_EQUAL(20 - 5, holding_.ShareCount());
}

TEST(AHolding, ShareCountCanBeNegative)  {
   PurchaseRecord p(-5, arbitraryDate);
   holding_.Add(p);

   LONGS_EQUAL(-5, holding_.ShareCount());
}
