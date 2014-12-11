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

TEST(SoundexEncoding, IgnoresNonAlphabetics) {
   CHECK_EQUAL(string("F234"), soundex.encode("F987654321%#.=+cdl"));
}

IGNORE_TEST(SoundexEncoding, CombinesDuplicateEncodings) {
   CHECK_EQUAL(soundex.codeFor('f'), soundex.codeFor('b'));
   CHECK_EQUAL(soundex.codeFor('g'), soundex.codeFor('c'));
   CHECK_EQUAL(soundex.codeFor('t'), soundex.codeFor('d'));

   CHECK_EQUAL(string("A123"), soundex.encode("Abfcgdt"));
}

TEST(SoundexEncoding, LimitsLengthToFourCharacters) {
   CHECK_EQUAL(4u, soundex.encode("Dcdlb").length());
}
