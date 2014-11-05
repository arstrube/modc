#include "PersistenceTest.h"


#include <memory>
#include "KeyedMemoryPersistence.h"
#include "TestSerializable.h"

using namespace std;

TEST_GROUP(PersistenceTest) {
    PersistenceTestFixture f;
    
    void setup() override {
        f.SetUp();
    }
    
    void teardown() override {
        f.TearDown();
    }
};

TEST(PersistenceTest, IsEmptyOnCreation)
{
    LONGS_EQUAL(0u, f.persister->Size());
}

/*
TEST(PersistenceTest, SizeSetToOneOnFirstAdd)
{
    f.persister->Add(*f.objectWithId1);

    LONGS_EQUAL(1u, f.persister->Size());
}

TEST(PersistenceTest, SizeIncrementsWithEachAdd)
{
    persister->Add(*objectWithId1);
    persister->Add(*objectWithId2);

    ASSERT_THAT(persister->Size(), Eq(2u));
}

TEST(PersistenceTest, ReturnsNullPointerWhenItemNotFound)
{
    auto_ptr<TestSerializable> found = persister->Get("1");

    TestSerializable* serializable = found.get();

    ASSERT_THAT(serializable, IsNull());
}

TEST(PersistenceTest, AddedItemCanBeRetrievedById)
{
    persister->Add(*objectWithId1);

    ASSERT_THAT(*persister->Get("1"), Eq(*objectWithId1));
}

TEST(PersistenceTest, GetAnswersNullWhenNoMatchingEntries)
{
    ASSERT_THAT(persister->Get("1").get(), IsNull());
};

TEST(PersistenceTest, RetrievedItemIsNewInstance)
{
    persister->Add(*objectWithId1);

    ASSERT_FALSE(objectWithId1 == persister->Get("1").get());
}

TEST(PersistenceTest, CanPersistMultipleObjects)
{
    persister->Add(*objectWithId1);
    persister->Add(*objectWithId2);

    ASSERT_THAT(*(persister->Get("1")), Eq(*objectWithId1));
    ASSERT_THAT(*(persister->Get("2")), Eq(*objectWithId2));
}

bool NameMatcher(Serializable& each, const string& name)
{
    return (dynamic_cast<TestSerializable&>(each)).Name() == name;
}

TEST(PersistenceTest, MatchesAnswersTrueWithMatchingEntries)
{
    persister->Add(*objectWithId1);
    persister->Add(*objectWithId2);
    string object1Name = objectWithId1->Name();

    bool exists = persister->Matches(NameMatcher, object1Name);

    ASSERT_TRUE(exists);
};

TEST(PersistenceTest, MatchesAnswersFalseWhenNoMatchingEntries)
{
    bool exists = persister->Matches(NameMatcher, "don't match anything");

    ASSERT_FALSE(exists);
};

TEST(PersistenceTest, FindAllMatching) 
{
    TestSerializable coolidge("Calvin", "1");
    TestSerializable langr("Jeff", "2");
    TestSerializable lynne("Jeff", "3");
    persister->Add(coolidge);
    persister->Add(langr);
    persister->Add(lynne);

    vector<Serializable*> matches;
    persister->FindAllMatching(NameMatcher, "Jeff", matches);

    ASSERT_THAT(matches.size(), Eq(2u));
};
*/
