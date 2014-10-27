#include "cpputest/testharness.h"
#include "Portfolio.h"

TEST_GROUP(APortfolio) {
   std::string IBM;
   Portfolio portfolio_;
   void setup() {
       IBM = "IBM";
   }
};

TEST(APortfolio, IsEmptyWhenCreated) {
   CHECK_TRUE(portfolio_.IsEmpty());
}

TEST(APortfolio, IsNotEmptyAfterPurchase) {
   portfolio_.Purchase(IBM, 1);
   CHECK_FALSE(portfolio_.IsEmpty());
}

TEST(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol) {
   LONGS_EQUAL(0u, portfolio_.ShareCount("AAPL"));
}

TEST(APortfolio, AnswersShareCountForPurchasedSymbol) {
   portfolio_.Purchase(IBM, 2);
   LONGS_EQUAL(2u, portfolio_.ShareCount("IBM"));
}

TEST(APortfolio, ThrowsOnPurchaseOfZeroShares) {
   try {
      portfolio_.Purchase(IBM, 0);
      FAIL("Expected exception but got none.");
   } 
   catch (InvalidPurchaseException expected) {}
}
