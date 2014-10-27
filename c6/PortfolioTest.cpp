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

#define ASSERT_THROW(method_call){\
   try {\
      method_call;\
      FAIL("Expected exception but got none.");\
   }\
   catch (std::exception e) {}\
}

#define ASSERT_THROW_E(method_call, expected_exception){\
   try {\
      method_call;\
      FAIL("Expected exception but got none.");\
   }\
   catch (expected_exception expected) {\
      ; /* Check type of exception here? */\
   }\
}

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
   ASSERT_THROW(portfolio_.Purchase(IBM, 0));
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
   ASSERT_THROW_E(portfolio_.Sell(SAMSUNG, 1), InvalidSellException);
}

TEST(APortfolio, AnswersThePurchaseRecordForASinglePurchase) {
   portfolio_.Purchase(SAMSUNG, 5);
   auto purchases = portfolio_.Purchases(SAMSUNG);

   auto purchase = purchases[0];
   LONGS_EQUAL(5u, purchase.ShareCount);
   CHECK(purchase.Date == Portfolio::FIXED_PURCHASE_DATE);
}
