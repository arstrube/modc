#include "ClassificationService.h"

#include "CppUTest.h"

TEST_GROUP(ClassificationServiceTest) {};

TEST(ClassificationServiceTest, ThrowsWhenNotFound)
{
    ClassificationService service;
    ASSERT_THROW(ClassificationNotFoundException, service.RetrieveDetails("nonexistent"));
}
