#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include "StringUtil.h"

using namespace std;
using namespace stringutil;

TEST_GROUP(AString) {};

TEST(AString, AnswersHeadAsItsFirstLetter) {
   STRCMP_EQUAL("x", head("xyz").c_str());
}

TEST(AString, AnswersHeadAsEmptyWhenEmpty) {
   STRCMP_EQUAL("", head("").c_str());
}

TEST(AString, AnswersTailAsAllLettersAfterHead) {
   STRCMP_EQUAL("yz", tail("xyz").c_str());
}

TEST(AString, AnswersTailAsEmptyWhenEmpty) {
   STRCMP_EQUAL("", tail("").c_str());
}

TEST(AString, AnswersTailAsEmptyWhenContainsOnlyOneCharacter) {
   STRCMP_EQUAL("", tail("X").c_str());
}

