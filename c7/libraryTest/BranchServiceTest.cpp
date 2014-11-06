#include "CppUTest/TestHarness.h"
#include "assert_throw.h"

#include "Branch.h"
#include "BranchService.h"

using std::string;

TEST_GROUP(BranchServiceTest)
{
public:
    Branch* eastBranch;
    Branch* westBranch;
    BranchService service;

    virtual void setup() override
    {
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads(); // TODO: Persistence.Remove()
        BranchService::DeleteAll();
        eastBranch = new Branch("1", "east");
        westBranch = new Branch("2", "west");
    }

    virtual void teardown() override
    {
        delete eastBranch;
        delete westBranch;
        BranchService::DeleteAll();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

TEST(BranchServiceTest, AddReturnsGeneratedId)
{
    string id = service.Add("newname", "address");

    CHECK(id > "0");
}

TEST(BranchServiceTest, AddGeneratesUniqueId)
{
    string id1 = service.Add("newname1", "address");
    string id2 = service.Add("newname2", "address");

    CHECK(id1 != id2);
}

TEST(BranchServiceTest, AddThrowsWhenNameNotUnique)
{
    service.Add("samename", "address1");

    ASSERT_THROW_E(service.Add("samename", "address2"), DuplicateBranchNameException);
}

TEST(BranchServiceTest, CountInitiallyZero)
{
    LONGS_EQUAL(0, service.BranchCount());
}

TEST(BranchServiceTest, DeleteAllRemovesAnyAddedBranches)
{
    service.Add(*eastBranch);
    service.Add(*westBranch);

    BranchService::DeleteAll();

    CHECK_FALSE(service.Find(*eastBranch));
    CHECK_FALSE(service.Find(*westBranch));
}

TEST(BranchServiceTest, DeleteAllSetsCountToZero)
{
    service.Add(*eastBranch);
    service.Add(*westBranch);

    BranchService::DeleteAll();

    LONGS_EQUAL(0, service.BranchCount());
}

TEST(BranchServiceTest, FindAnswersFalseForNonexistentBranch)
{
    CHECK_FALSE(service.Find(*eastBranch));
}

TEST(BranchServiceTest, FindAnswersTrueForAddedBranch)
{
    service.Add(*eastBranch);

    CHECK_TRUE(service.Find(*eastBranch));
}

TEST(BranchServiceTest, FindRetrievesById)
{
    service.Add(*eastBranch);

    Branch retrieved(eastBranch->Id(), "");
    service.Find(retrieved);

    STRCMP_EQUAL(eastBranch->Name().c_str(), retrieved.Name().c_str());
}

TEST(BranchServiceTest, AddBranchIncrementsCount)
{
    service.Add(*eastBranch);
    LONGS_EQUAL(1, service.BranchCount());

    service.Add(*westBranch);
    LONGS_EQUAL(2, service.BranchCount());
}

TEST(BranchServiceTest, PersistsAcrossServiceInstances)
{
    service.Add(*eastBranch);

    BranchService anotherServiceInstance;
    CHECK_TRUE(anotherServiceInstance.Find(*eastBranch));
    LONGS_EQUAL(1, anotherServiceInstance.BranchCount());
}
