#include "CppUTest.h"

#include "Branch.h"
#include "BranchService.h"

using std::string;

class BranchServiceTestFixture {
public:
    Branch* eastBranch;
    Branch* westBranch;
    BranchService service;

    BranchServiceTestFixture()
    {
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads(); // TODO: Persistence.Remove()
        eastBranch = new Branch("1", "east");
        westBranch = new Branch("2", "west");
    }

    ~BranchServiceTestFixture()
    {
        delete eastBranch;
        delete westBranch;
        BranchService::DeleteAll();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

TEST_GROUP(BranchServiceTest)
{
public:
    BranchServiceTestFixture f;
};

TEST(BranchServiceTest, AddReturnsGeneratedId)
{
    string id = f.service.Add("newname", "address");

    CHECK(id > "0");
}

TEST(BranchServiceTest, AddGeneratesUniqueId)
{
    string id1 = f.service.Add("newname1", "address");
    string id2 = f.service.Add("newname2", "address");

    CHECK(id1 != id2);
}

TEST(BranchServiceTest, AddThrowsWhenNameNotUnique)
{
    f.service.Add("samename", "address1");

    CHECK_THROWS(DuplicateBranchNameException, f.service.Add("samename", "address2"));
}

TEST_GROUP(ABranchServiceWithOneBranchAdded)
{
public:
    BranchServiceTestFixture f;
    Branch* BranchAlreadyAdded;
    void setup() {
        BranchAlreadyAdded = f.eastBranch;
        f.service.Add(*BranchAlreadyAdded);
    }
};

TEST(ABranchServiceWithOneBranchAdded, ThrowsWhenDuplicateBranchAdded)
{
   CHECK_THROWS(DuplicateBranchNameException, f.service.Add(BranchAlreadyAdded->Name(), ""));
}

TEST(BranchServiceTest, CountInitiallyZero)
{
    LONGS_EQUAL(0, f.service.BranchCount());
}

TEST(BranchServiceTest, DeleteAllRemovesAnyAddedBranches)
{
    f.service.Add(*f.eastBranch);
    f.service.Add(*f.westBranch);

    BranchService::DeleteAll();

    CHECK_FALSE(f.service.Find(*f.eastBranch));
    CHECK_FALSE(f.service.Find(*f.westBranch));
}

TEST(BranchServiceTest, DeleteAllSetsCountToZero)
{
    f.service.Add(*f.eastBranch);
    f.service.Add(*f.westBranch);

    BranchService::DeleteAll();

    LONGS_EQUAL(0, f.service.BranchCount());
}

TEST(BranchServiceTest, FindAnswersFalseForNonexistentBranch)
{
    CHECK_FALSE(f.service.Find(*f.eastBranch));
}

TEST(BranchServiceTest, FindAnswersTrueForAddedBranch)
{
    f.service.Add(*f.eastBranch);

    CHECK_TRUE(f.service.Find(*f.eastBranch));
}

TEST(BranchServiceTest, FindRetrievesById)
{
    f.service.Add(*f.eastBranch);

    Branch retrieved(f.eastBranch->Id(), "");
    f.service.Find(retrieved);

    STRCMP_EQUAL(f.eastBranch->Name().c_str(), retrieved.Name().c_str());
}

TEST(BranchServiceTest, AddBranchIncrementsCount)
{
    f.service.Add(*f.eastBranch);
    LONGS_EQUAL(1, f.service.BranchCount());

    f.service.Add(*f.westBranch);
    LONGS_EQUAL(2, f.service.BranchCount());
}

TEST(BranchServiceTest, PersistsAcrossServiceInstances)
{
    f.service.Add(*f.eastBranch);

    BranchService anotherServiceInstance;
    CHECK_TRUE(anotherServiceInstance.Find(*f.eastBranch));
    LONGS_EQUAL(1, anotherServiceInstance.BranchCount());
}
