#include "PersistenceTest.h"


#include <memory>
#include "KeyedMemoryPersistence.h"
#include "TestSerializable.h"

using namespace std;

TEST_GROUP(PersistenceTest) {
    PersistenceTestFixture f;

    void setup() override {
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads(); // TODO: Persistence.Remove()
        f.SetUp();
    }

    void teardown() override {
        f.TearDown();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

TEST(PersistenceTest, IsEmptyOnCreation)
{
    LONGS_EQUAL(0u, f.persister->Size());
}

TEST(PersistenceTest, SizeSetToOneOnFirstAdd)
{
    f.persister->Add(*f.objectWithId1);

    LONGS_EQUAL(1u, f.persister->Size());
}

TEST(PersistenceTest, SizeIncrementsWithEachAdd)
{
    f.persister->Add(*f.objectWithId1);
    f.persister->Add(*f.objectWithId2);

    LONGS_EQUAL(2u, f.persister->Size());
}

TEST(PersistenceTest, ReturnsNullPointerWhenItemNotFound)
{
    POINTERS_EQUAL(NULL, f.persister->Get("no id there").get());
}

TEST(PersistenceTest, AddedItemCanBeRetrievedById)
{
    f.persister->Add(*f.objectWithId1);

    auto found = f.persister->Get("1").get();

    CHECK(NULL != found);
    CHECK(*f.objectWithId1==*f.persister->Get("1"));
}

TEST(PersistenceTest, GetAnswersNullWhenNoMatchingEntries)
{
    POINTERS_EQUAL(NULL, f.persister->Get("1").get());
};

TEST(PersistenceTest, RetrievedItemIsNewInstance)
{
    f.persister->Add(*f.objectWithId1);

    CHECK_FALSE(f.objectWithId1 == f.persister->Get("1").get());
}

TEST(PersistenceTest, CanPersistMultipleObjects)
{
    f.persister->Add(*f.objectWithId1);
    f.persister->Add(*f.objectWithId2);

    CHECK(*f.objectWithId1==*(f.persister->Get("1")));
    CHECK(*f.objectWithId2==*(f.persister->Get("2")));
}

bool NameMatcher(Serializable& each, const string& name)
{
    return (dynamic_cast<TestSerializable&>(each)).Name() == name;
}

TEST(PersistenceTest, MatchesAnswersTrueWithMatchingEntries)
{
    f.persister->Add(*f.objectWithId1);
    f.persister->Add(*f.objectWithId2);
    string object1Name = f.objectWithId1->Name();

    bool exists = f.persister->Matches(NameMatcher, object1Name);

    CHECK_TRUE(exists);
};

TEST(PersistenceTest, MatchesAnswersFalseWhenNoMatchingEntries)
{
    bool exists = f.persister->Matches(NameMatcher, "don't match anything");

    CHECK_FALSE(exists);
};

TEST(PersistenceTest, FindAllMatching)
{
    TestSerializable coolidge("Calvin", "1");
    TestSerializable langr("Jeff", "2");
    TestSerializable lynne("Jeff", "3");
    f.persister->Add(coolidge);
    f.persister->Add(langr);
    f.persister->Add(lynne);

    vector<Serializable*> matches;
    f.persister->FindAllMatching(NameMatcher, "Jeff", matches);

    LONGS_EQUAL(2u, matches.size());
};
