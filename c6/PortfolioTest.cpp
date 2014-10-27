#include "cpputest/testharness.h"
#include "Portfolio.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"

using namespace boost::gregorian;

static const date arbitraryDate(2014, Sep, 5);

TEST_GROUP(APortfolio) {
   std::string IBM, SAMSUNG;
   Portfolio portfolio_;
   
   void setup() {
       IBM = "IBM";
       SAMSUNG = "SSNLF";
       
   }
   void purchase(
         const std::string& symbol, 
         unsigned int shareCount,
         const date& transactionDate=arbitraryDate) {
      portfolio_.Purchase(symbol, shareCount, transactionDate);
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
   purchase(IBM, 1);
   CHECK_FALSE(portfolio_.IsEmpty());
}

TEST(APortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol) {
   LONGS_EQUAL(0u, portfolio_.ShareCount("AAPL"));
}

TEST(APortfolio, AnswersShareCountForPurchasedSymbol) {
   portfolio_.Purchase(IBM, 2, arbitraryDate);
   LONGS_EQUAL(2u, portfolio_.ShareCount("IBM"));
}

TEST(APortfolio, ThrowsOnPurchaseOfZeroShares) {
   ASSERT_THROW(purchase(IBM, 0));
}

TEST(APortfolio, AnswersShareCountForAppropriateSymbol) {
   portfolio_.Purchase(IBM, 5, arbitraryDate);
   portfolio_.Purchase(SAMSUNG, 10, arbitraryDate);

   LONGS_EQUAL(5u, portfolio_.ShareCount(IBM));
}

TEST(APortfolio, ShareCountReflectsAccumulatedPurchasesOfSameSymbol) {
   purchase(IBM, 5);
   purchase(IBM, 15);

   LONGS_EQUAL(5u + 15, portfolio_.ShareCount(IBM));
}

TEST(APortfolio, ReducesShareCountOfSymbolOnSell)  {
   purchase(SAMSUNG, 30);
   portfolio_.Sell(SAMSUNG, 13);

   LONGS_EQUAL(30u - 13, portfolio_.ShareCount(SAMSUNG));
}

TEST(APortfolio, ThrowsWhenSellingMoreSharesThanPurchased) {
   ASSERT_THROW_E(portfolio_.Sell(SAMSUNG, 1), InvalidSellException);
}

TEST(APortfolio, AnswersThePurchaseRecordForASinglePurchase) {
   date dateOfPurchase(2014, Mar, 17);
   purchase(SAMSUNG, 5, dateOfPurchase);
   auto purchases = portfolio_.Purchases(SAMSUNG);

   auto purchase = purchases[0];
   LONGS_EQUAL(5u, purchase.ShareCount);
   CHECK(purchase.Date == dateOfPurchase);
}
