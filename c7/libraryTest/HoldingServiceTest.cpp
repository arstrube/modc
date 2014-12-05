#include "CppUTest.h"

#include "HoldingService.h"
#include "Branch.h"
#include "BranchService.h"
#include "ClassificationData.h"
#include "Patron.h"
#include "PatronService.h"
#include "HoldingBarcode.h"

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include "boost/assign/list_of.hpp"

using namespace std;
using namespace ClassificationData;
using namespace boost::assign;
using namespace boost::gregorian;
using namespace service;

TEST_GROUP(HoldingServiceTest)
{
public:
    Branch* branch1;
    Branch* branch2;
    PatronService* patronService;
    HoldingService* holdingService;
    BranchService branchService;
    date* arbitraryDate;

	virtual void setup() override
	{
	    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads(); // PatronService::PatronService()

        arbitraryDate = new date(2013, Jan, 1);
        patronService = new PatronService;
        holdingService = new HoldingService;
        branch1 = new Branch("1", "branch1");
	    branch2 = new Branch("2", "branch2");
        branchService.Add(*branch1);
        branchService.Add(*branch2);
	}

	virtual void teardown() override
	{
        HoldingService::DeleteAll();
        PatronService::DeleteAll();
        BranchService::DeleteAll();

		delete branch1;
		delete branch2;
		delete arbitraryDate;
		delete holdingService;
	    delete patronService;

	    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
	}

    void AddPatronWithId(string id)
    {
        Patron patron("", id);
        patronService->Add(patron);
    }

    Patron FindPatronWithId(string id)
    {
        Patron patron("", id);
        patronService->Find(patron);
        return patron;
    }

    Holding FindHolding(HoldingBarcode& barcode)
    {
        Holding holding(barcode.AsString());
        holdingService->FindByBarCode(holding);
        return holding;
    }

    Holding FindHolding(const string& barcode)
    {
        Holding holding(barcode);
        holdingService->FindByBarCode(holding);
        return holding;
    }

    void CheckOut(HoldingBarcode& barcode, Branch* branch, string patronCardNumber="p1000")
    {
        AddPatronWithId(patronCardNumber);
        holdingService->AddAtBranch(branch->Id(), barcode.AsString());
        holdingService->CheckOut(patronCardNumber, barcode.AsString(), *arbitraryDate);
    }
};

TEST(HoldingServiceTest, SizeInitiallyZero)
{
    LONGS_EQUAL(0u, holdingService->InventorySize());
}

TEST(HoldingServiceTest, SizeIncrementedOnAddRegardlessOfBranch)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    LONGS_EQUAL(1u, holdingService->InventorySize());

    holdingService->AddAtBranch(branch2->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 2).AsString());
    LONGS_EQUAL(2u, holdingService->InventorySize());
}

TEST(HoldingServiceTest, DeleteAllSetsSizeToZero)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    holdingService->AddAtBranch(branch2->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 2).AsString());

	HoldingService::DeleteAll();

    LONGS_EQUAL(0u, holdingService->InventorySize());
}

TEST(HoldingServiceTest, AddInitializesBranch)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    Holding holding(THE_TRIAL_CLASSIFICATION, 1);

    holdingService->FindByBarCode(holding);

    CHECK_TRUE(holding.CurrentBranch() == *branch1);
}

TEST(HoldingServiceTest, AddedHoldingCanBeFound)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());

    Holding theTrial(THE_TRIAL_CLASSIFICATION, 1);
    CHECK_TRUE(holdingService->FindByBarCode(theTrial));
}

TEST(HoldingServiceTest, ExistsReturnsFalseWhenNotFound)
{
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    bool found = holdingService->ExistsWithBarcode(barcode);

    CHECK_FALSE(found);
}

TEST(HoldingServiceTest, ExistsReturnsTrueWhenNotFound)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    bool found = holdingService->ExistsWithBarcode(barcode);

    CHECK_TRUE(found);
}

TEST(HoldingServiceTest, IsAvailableReturnsTrueWhenHoldingAvailable)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    bool isAvailable = holdingService->IsAvailable(barcode);

    CHECK_TRUE(isAvailable);
}

TEST(HoldingServiceTest, IsAvailableReturnsFalseWhenHoldingCheckedOut)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    CheckOut(barcode, branch1);

    bool isAvailable = holdingService->IsAvailable(barcode.AsString());

    CHECK_FALSE(isAvailable);
}

TEST(HoldingServiceTest, FindByClassificationReturnsEmptyWhenNoMatch)
{
    set<Holding> holdings;

    holdingService->FindByClassification(THE_TRIAL_CLASSIFICATION, holdings);

    CHECK_TRUE(holdings.empty());
}

TEST(HoldingServiceTest, FindByClassificationReturnsMultipleMatches)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 2).AsString());
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    set<Holding> holdings;

    holdingService->FindByClassification(THE_TRIAL_CLASSIFICATION, holdings);

    Holding trialCopy1(THE_TRIAL_CLASSIFICATION, 1);
    Holding trialCopy2(THE_TRIAL_CLASSIFICATION, 2);
    CHECK_TRUE(holdings == list_of(trialCopy1)(trialCopy2));
}

TEST(HoldingServiceTest, Transfer)
{
	holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    holdingService->Transfer(barcode, branch1->Id());

	CHECK_TRUE(FindHolding(barcode).CurrentBranch() == *branch1);
}

TEST(HoldingServiceTest, CheckedOutHoldingUnavailable)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    AddPatronWithId("p1001");

    holdingService->CheckOut("p1001", HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString(), *arbitraryDate);

	Holding retrieved(HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
	holdingService->FindByBarCode(retrieved);
	CHECK_FALSE(retrieved.IsAvailable());
}

TEST(HoldingServiceTest, CheckedOutBooksAddedToPatron)
{
    holdingService->AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString();
    AddPatronWithId("p1001");

    holdingService->CheckOut("p1001", HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString(), *arbitraryDate);

    Holding holding(HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    std::set<Holding> expected_holdings;
    CHECK_TRUE(FindPatronWithId("p1001").Holdings() == list_of(holding));
}

TEST(HoldingServiceTest, CheckInUpdatesHoldingBranch)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    CheckOut(barcode, branch1);

    holdingService->CheckIn(barcode.AsString(), *arbitraryDate + date_duration(1), branch2->Id());

    CHECK_TRUE(FindHolding(barcode).CurrentBranch() == *branch2);
}

TEST(HoldingServiceTest, CheckInUpdatesPatronHoldings)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    string patronId("p5");
    CheckOut(barcode, branch1, patronId);

    holdingService->CheckIn(barcode.AsString(), *arbitraryDate + date_duration(1), branch2->Id());

    Patron retrieved = FindPatronWithId(patronId);
    LONGS_EQUAL(0u, retrieved.Holdings().size());
}

TEST(HoldingServiceTest, CheckInEarlyDoesNotUpdatePatronFineBalance)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    string patronCardNumber("p5");
    CheckOut(barcode, branch1, patronCardNumber);

    holdingService->CheckIn(barcode.AsString(), *arbitraryDate + date_duration(1), branch2->Id());

    DOUBLES_EQUAL(0.0f, FindPatronWithId(patronCardNumber).FineBalance(), 0.01);
}

TEST(HoldingServiceTest, X)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    string patronCardNumber("p5");
    CheckOut(barcode, branch1, patronCardNumber);
    date_duration oneDayLate(Book::BOOK_CHECKOUT_PERIOD + 1);

    holdingService->CheckIn(barcode.AsString(), *arbitraryDate + oneDayLate, branch2->Id());

    CHECK_TRUE(FindPatronWithId(patronCardNumber).FineBalance() == Book::BOOK_DAILY_FINE);
}
