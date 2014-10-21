#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include <string>
#include "CharUtil.h"

using namespace std;
using namespace charutil;

TEST_GROUP(AChar) {};

TEST(AChar, IsAVowelForSixSpecificLetters) {
   CHECK(isVowel('A'));
   CHECK(isVowel('E'));
   CHECK(isVowel('I'));
   CHECK(isVowel('O'));
   CHECK(isVowel('U'));
   CHECK(isVowel('Y'));
}

TEST(AChar, IsAVowelForLowercaseLetters) {
   CHECK(isVowel('a'));
   CHECK(isVowel('e'));
   CHECK(isVowel('i'));
   CHECK(isVowel('o'));
   CHECK(isVowel('u'));
   CHECK(isVowel('y'));
}

TEST(AChar, IsNotAVowelForAnyOtherCharacter) {
   CHECK_FALSE(isVowel('b'));
}

TEST(AChar, AnswersAppropriateUpperCaseLetter) {
   CHECK(upper('a') == 'A');
}

TEST(AChar, HandlesAlreadyUppercasedLetters) {
   CHECK(upper('B') == 'B');
}

TEST(AChar, IgnoresNonLettersWhenUppercasing) {
  CHECK(upper('+') == '+');
}

TEST(AChar, AnswersAppropriateLowerCaseLetter) {
   CHECK(lower('A') == 'a');
}

TEST(AChar, HandlesAlreadyLowercased) {
   CHECK(lower('b') == 'b');
}

TEST(AChar, IgnoresNonLettersWhenLowercasing) {
   CHECK(lower('+') == '+');
}
