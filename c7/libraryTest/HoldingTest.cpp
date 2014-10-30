#include "CppUTest/TestHarness.h"
#include "Holding.h"
#include "Book.h"
#include "ClassificationData.h"
#include "BranchTestData.h"
#include "Branch.h"
#include "InvalidBarcodeException.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"

using namespace boost::gregorian;
using namespace BranchTestData;
using namespace ClassificationData;
using namespace std;

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

class HoldingTestFixture
{
public:
   static const date ArbitraryDate;
   Holding* holding;
   Branch* arbitraryBranch;

   void VerifyAvailability(const Branch& branch) {
      CHECK(holding->CurrentBranch() == branch);
      CHECK(holding->IsAvailable() == (branch != Branch::CHECKED_OUT));
   }

    bool IsAvailableAt(Holding* holding, Branch& branch) {
        return holding->CurrentBranch() == branch &&
               holding->IsAvailable();
    }

    void MakeAvailableAtABranch(Holding* holding) {
        holding->Transfer(EAST_BRANCH);
    }
};

const date HoldingTestFixture::ArbitraryDate(2014, Jan, 1);

TEST_GROUP(HoldingTest)
{
public:
   HoldingTestFixture f;

   virtual void setup() override {
      f.holding = new Holding(THE_TRIAL_CLASSIFICATION, 1);
      f.arbitraryBranch = new Branch(EAST_BRANCH);
   }

   virtual void teardown() override {
      delete f.holding;
      delete f.arbitraryBranch;
   }
};

TEST(HoldingTest, BarcodeRequiresColon)
{
    ASSERT_THROW_E(Holding("A"), InvalidBarcodeException);
}

TEST(HoldingTest, CanExtractClassificationWhenCreatedWithBarcode)
{
    Holding holding(Holding::ConstructBarcode("A123", 3));

    STRCMP_EQUAL("A123", holding.Classification().c_str());
}

TEST(HoldingTest, CanExtractCopyNumberWhenCreatedWithBarcode)
{
    Holding holding(Holding::ConstructBarcode("A234", 5));

    LONGS_EQUAL(5u, holding.CopyNumber());
}

TEST(HoldingTest, IsNotAvailableWhenCreated)
{
    Holding holding(Holding::ConstructBarcode("A", 1));

    CHECK_FALSE(holding.IsAvailable());
}

TEST(HoldingTest, CanSpecifyClassificationSeparatelyWhenCreated)
{
    Holding holding("B", 2);

    STRCMP_EQUAL("B", holding.Classification().c_str());
}

TEST(HoldingTest, CanSpecifyCopyNumberSeparatelyWhenCreated)
{
    Holding holding("A", 7);

    LONGS_EQUAL(7, holding.CopyNumber());
}

TEST(HoldingTest, IsNotAvailableWhenCreatedWithSeparateClassificationAndCopy)
{
    Holding holding("A", 2);

    CHECK_FALSE(holding.IsAvailable());
}

TEST(HoldingTest, AssignmentCopiesAllMembers)
{
   f.holding->Transfer(EAST_BRANCH);

   Holding newHolding = *f.holding;

   STRCMP_EQUAL(THE_TRIAL_CLASSIFICATION.c_str(), newHolding.Classification().c_str());
   LONGS_EQUAL(f.holding->CopyNumber(), newHolding.CopyNumber());
   CHECK(f.IsAvailableAt(&newHolding, EAST_BRANCH));
}

TEST(HoldingTest, TransferMakesHoldingAvailableAtBranch)
{
   f.holding->Transfer(EAST_BRANCH);

   CHECK_TRUE(f.IsAvailableAt(f.holding, EAST_BRANCH));
}

TEST(HoldingTest, BarCodeCombinesClassificationAndCopyNumber)
{
   Holding holding(THE_TRIAL_CLASSIFICATION, 5);

   string barcode = holding.Barcode();

   STRCMP_EQUAL((THE_TRIAL_CLASSIFICATION + ":5").c_str(), barcode.c_str());
}

TEST(HoldingTest, AreEqualWhenClassificationAndCopyMatch)
{
   Holding copy(f.holding->Classification(), f.holding->CopyNumber());

   CHECK_TRUE(*f.holding == copy);
}

TEST(HoldingTest, AreUnequalWhenCopyDoesNotMatch)
{
   Holding extraCopy(f.holding->Classification(), f.holding->CopyNumber() + 1);

   CHECK_TRUE(*f.holding != extraCopy);
}

TEST(HoldingTest, AreUnequalWhenClassificationDoesNotMatch)
{
   Holding differentBook(f.holding->Classification() + "X", 1);

   CHECK_TRUE(*f.holding != differentBook);
}

TEST(HoldingTest, IsLessThanWhenClassificationsAreLessThan)
{
   Holding a("A:1");
   Holding b("B:1");

   CHECK_TRUE(a < b);
}

TEST(HoldingTest, IsNotLessThanWhenClassificationIsNotLessThan)
{
    Holding a("A:1");
    Holding b("B:1");

    CHECK_TRUE(!(b < a));
}

TEST(HoldingTest, IsNotLessThanWhenBarcodesAreEqual)
{
    Holding a("A:1");
    Holding aCopy("A:1");

    CHECK_FALSE(a < aCopy);
}

TEST(HoldingTest, ck)
{
   f.holding->Transfer(EAST_BRANCH);

   date checkoutOn(2007, Mar, 1);
   f.holding->CheckOut(checkoutOn);

   CHECK_FALSE(f.holding->IsAvailable());

   CHECK_TRUE(f.holding->LastCheckedOutOn() == checkoutOn);

   date_duration daysCheckedOut(Book::BOOK_CHECKOUT_PERIOD);
   date expectedDue = checkoutOn + daysCheckedOut;
   CHECK_TRUE(f.holding->DueDate() == expectedDue);
}

TEST(HoldingTest, Availability)
{
   f.holding->Transfer(EAST_BRANCH);
   f.holding->CheckOut(f.ArbitraryDate);
   CHECK_FALSE(f.holding->IsAvailable());

   date nextDay = f.ArbitraryDate + date_duration(1);
   f.holding->CheckIn(nextDay, EAST_BRANCH);
   CHECK_TRUE(f.holding->IsAvailable());
}

TEST(HoldingTest, UnavailableOnCheckout)
{
    f.MakeAvailableAtABranch(f.holding);

    f.holding->CheckOut(f.ArbitraryDate);

    CHECK_FALSE(f.holding->IsAvailable());
}

TEST(HoldingTest, UpdatesCheckoutDateOnCheckout)
{
    f.MakeAvailableAtABranch(f.holding);

    f.holding->CheckOut(f.ArbitraryDate);

    CHECK_TRUE(f.holding->LastCheckedOutOn() == f.ArbitraryDate);
}
#if 0
TEST_GROUP(ACheckedInHolding)
{
public:
   void SetUp() override {
      HoldingTest::SetUp();
      MakeAvailableAtABranch(holding);
   }
};

TEST(ACheckedInHolding, UpdatesDateDueOnCheckout)
{
   ASSERT_TRUE(IsAvailableAt(holding, *arbitraryBranch));
   holding->CheckOut(ArbitraryDate);

   ASSERT_THAT(holding->DueDate(),
      Eq(ArbitraryDate + date_duration(Book::BOOK_CHECKOUT_PERIOD)));
}

TEST(HoldingTest, UpdatesDateDueOnCheckout)
{
    MakeAvailableAtABranch(holding);

    holding->CheckOut(ArbitraryDate);

    ASSERT_THAT(holding->DueDate(),
        Eq(ArbitraryDate + date_duration(Book::BOOK_CHECKOUT_PERIOD)));
}

TEST(HoldingTest, Ckin)
{
   holding->Transfer(EAST_BRANCH);
   date checkoutOn(2007, Mar, 1);
   holding->CheckOut(checkoutOn);

   date checkinOn(2007, Mar, 2);
   Branch branch2("2", "branch2");
   holding->CheckIn(checkinOn, branch2);

   ASSERT_TRUE(IsAvailableAt(holding, branch2));
}

TEST(HoldingTest, CheckinMakesBookAvailableAtAnotherBranch)
{
   holding->Transfer(EAST_BRANCH);
   holding->CheckOut(ArbitraryDate);

   holding->CheckIn(
       ArbitraryDate + date_duration(1), WEST_BRANCH);

   ASSERT_TRUE(IsAvailableAt(holding, WEST_BRANCH));
}

TEST(HoldingTest, DateDueForMovies)
{
   Holding holdingA(SEVEN_CLASSIFICATION, 1);
   holdingA.Transfer(EAST_BRANCH);
   date checkoutOn(2007, Mar, 1);
   holdingA.CheckOut(checkoutOn);

   ASSERT_THAT(holdingA.DueDate(), Eq(checkoutOn + date_duration(Book::MOVIE_CHECKOUT_PERIOD)));
}

TEST(HoldingTest, MoviesDueCheckoutPeriodDaysAfterCheckout)
{
    Holding movie(SEVEN_CLASSIFICATION, 1);
    MakeAvailableAtABranch(&movie);

    movie.CheckOut(ArbitraryDate);

    ASSERT_THAT(movie.DueDate(),
        Eq(ArbitraryDate + date_duration(Book::MOVIE_CHECKOUT_PERIOD)));
}
#endif
