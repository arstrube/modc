#include "CppUTest/TestHarness.h"

#include "Soundex.h"

using namespace std;

TEST_GROUP(SoundexEncoding) {
   Soundex soundex;
};

TEST(SoundexEncoding, AppendsZerosToWordForOneLetterWord) {
   CHECK_EQUAL(string("A000"), soundex.encode("A"));
   CHECK_EQUAL(string("B000"), soundex.encode("B"));
}

TEST(SoundexEncoding, ReplacesConsonantsWithAppropriateDigits) {
   CHECK_EQUAL(string("A100"), soundex.encode("Ab"));
   CHECK_EQUAL(string("A200"), soundex.encode("Ac"));
}

TEST(SoundexEncoding, ReplacesMultipleConsonantsWithDigits) {
   CHECK_EQUAL(string("A234"), soundex.encode("Acdl"));
}

TEST(SoundexEncoding, IgnoresVowelLikeLetters) {
   CHECK_EQUAL(string("B234"), soundex.encode("BAaEeIiOoUuHhYycdl"));
}

