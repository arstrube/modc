#include "cpputest/testharness.h"
#include "Portfolio.h"

TEST_GROUP(APortfolio) {
   std::string IBM, SAMSUNG;
   Portfolio portfolio_;
   void setup() {
       IBM = "IBM";
       SAMSUNG = "SSNLF";
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
   catch (InvalidPurchaseException e) {}
}
TEST(APortfolio, AnswersShareCountForAppropriateSymbol) {
   portfolio_.Purchase(IBM, 5);
   portfolio_.Purchase(SAMSUNG, 10);

   LONGS_EQUAL(5u, portfolio_.ShareCount(IBM));
}

TEST(APortfolio, ShareCountReflectsAccumulatedPurchasesOfSameSymbol) {
   portfolio_.Purchase(IBM, 5);
   portfolio_.Purchase(IBM, 15);

   LONGS_EQUAL(5u + 15, portfolio_.ShareCount(IBM));
}

TEST(APortfolio, ReducesShareCountOfSymbolOnSell)  {
   portfolio_.Purchase(SAMSUNG, 30);
   portfolio_.Sell(SAMSUNG, 13);

   LONGS_EQUAL(30u - 13, portfolio_.ShareCount(SAMSUNG));
}

TEST(APortfolio, ThrowsWhenSellingMoreSharesThanPurchased) {
   try {
      portfolio_.Sell(SAMSUNG, 1);
   }
   catch (InvalidSellException e) {}
}
