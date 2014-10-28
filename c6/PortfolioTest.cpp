#include "cpputest/testharness.h"
#include "Portfolio.h"
#include "PurchaseRecord.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"
#include "DateTestConstants.h"
#include <vector>

using namespace boost::gregorian;

bool operator==(const PurchaseRecord& lhs, const PurchaseRecord& rhs) {
      return lhs.ShareCount == rhs.ShareCount && lhs.Date == rhs.Date;
}

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
   void sell(
         const std::string& symbol, 
         unsigned int shareCount,
         const date& transactionDate=arbitraryDate) {
      portfolio_.Sell(symbol, shareCount, transactionDate);
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

#define ASSERT_PURCHASE(purchaseRecord, shareCount, date) {\
   LONGS_EQUAL(shareCount, purchaseRecord.ShareCount);\
   CHECK_TRUE(purchaseRecord.Date == date);\
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
   purchase(IBM, 2);
   LONGS_EQUAL(2u, portfolio_.ShareCount("IBM"));
}

TEST(APortfolio, ThrowsOnPurchaseOfZeroShares) {
   ASSERT_THROW(purchase(IBM, 0));
}

TEST(APortfolio, AnswersShareCountForAppropriateSymbol) {
   purchase(IBM, 5, arbitraryDate);
   purchase(SAMSUNG, 10, arbitraryDate);

   LONGS_EQUAL(5u, portfolio_.ShareCount(IBM));
}

TEST(APortfolio, ShareCountReflectsAccumulatedPurchasesOfSameSymbol) {
   purchase(IBM, 5);
   purchase(IBM, 15);

   LONGS_EQUAL(5u + 15, portfolio_.ShareCount(IBM));
}

TEST(APortfolio, ReducesShareCountOfSymbolOnSell)  {
   purchase(SAMSUNG, 30);
   sell(SAMSUNG, 13);

   LONGS_EQUAL(30u - 13, portfolio_.ShareCount(SAMSUNG));
}

TEST(APortfolio, ThrowsWhenSellingMoreSharesThanPurchased) {
   ASSERT_THROW_E(sell(SAMSUNG, 1), InsufficientSharesException);
}

TEST(APortfolio, AnswersThePurchaseRecordForASinglePurchase) {
   purchase(SAMSUNG, 5, arbitraryDate);
   auto purchases = portfolio_.Purchases(SAMSUNG);

   ASSERT_PURCHASE(purchases[0], 5, arbitraryDate);
}

TEST(APortfolio, IncludesSalesInPurchaseRecords) {
   purchase(SAMSUNG, 10);
   sell(SAMSUNG, 5, arbitraryDate);

   auto sales = portfolio_.Purchases(SAMSUNG);
   ASSERT_PURCHASE(sales[1], -5, arbitraryDate);
}

TEST(APortfolio, ThrowsOnSellOfZeroShares) {
   ASSERT_THROW_E(sell(IBM, 0), ShareCountCannotBeZeroException);
}

#define CHECK_ELEMENT(set, record) { \
   auto fail = true; \
   for(auto it : set) if(it == record ) { \
      fail = false; \
      break; \
   } \
   if(fail) FAIL("Record not found"); \
}

TEST(APortfolio, SeparatesPurchaseRecordsBySymbol) {
   purchase(SAMSUNG, 5, arbitraryDate);
   purchase(IBM, 1, arbitraryDate);

   auto sales = portfolio_.Purchases(SAMSUNG);
   CHECK_ELEMENT(sales, PurchaseRecord(5, arbitraryDate));
}

TEST(APortfolio, AnswersEmptyPurchaseRecordVectorWhenSymbolNotFound) {
   CHECK(portfolio_.Purchases(SAMSUNG)==std::vector<PurchaseRecord>())
}

TEST(APortfolio, SupportsMultiplePurchaseRecordsOfSymbol) {
   purchase(SAMSUNG, 4, arbitraryDate);
   purchase(SAMSUNG, 11, arbitraryDate);

   auto sales = portfolio_.Purchases(SAMSUNG);

   CHECK_ELEMENT(sales, PurchaseRecord(4, arbitraryDate));
   CHECK_ELEMENT(sales, PurchaseRecord(11, arbitraryDate));
}
