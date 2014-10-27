#include "cpputest/testharness.h"
#include "Portfolio.h"

TEST_GROUP(APortfolio) {
   Portfolio portfolio_;
};

TEST(APortfolio, IsEmptyWhenCreated) {
   CHECK_TRUE(portfolio_.IsEmpty());
}

TEST(APortfolio, IsNotEmptyAfterPurchase) {
   portfolio_.Purchase("IBM", 1);

   CHECK_FALSE(portfolio_.IsEmpty());
}

TEST(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol) {
   LONGS_EQUAL(0u, portfolio_.ShareCount("AAPL"));
}

TEST(APortfolio, AnswersShareCountForPurchasedSymbol) {
   portfolio_.Purchase("IBM", 2);
   LONGS_EQUAL(2u, portfolio_.ShareCount("IBM"));
}
