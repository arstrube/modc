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

TEST(SoundexEncoding, CombinesDuplicateEncodings) {
   CHECK_EQUAL(soundex.codeFor('f'), soundex.codeFor('b'));
   CHECK_EQUAL(soundex.codeFor('g'), soundex.codeFor('c'));
   CHECK_EQUAL(soundex.codeFor('t'), soundex.codeFor('d'));

   CHECK_EQUAL(string("A123"), soundex.encode("Abfcgdt"));
}

TEST(SoundexEncoding, CombinesMultipleDuplicateEncodings) {
   CHECK_EQUAL(string("J100"), soundex.encode("Jbbb"));
}

TEST(SoundexEncoding, CombinesDuplicateCodesWhen2ndLetterDuplicates1st) {
   CHECK_EQUAL(string("B230"), soundex.encode("bbcd"));
}

TEST(SoundexEncoding, UppercasesFirstLetter) {
   CHECK_EQUAL(string("A"), soundex.encode("abcd").substr(0, 1));
}

TEST(SoundexEncoding, IgnoresCaseWhenEncodingConsonants) {
   CHECK_EQUAL(soundex.encode("BCDL"), soundex.encode("bcdl"));
}

TEST(SoundexEncoding, DoesNotCombineDuplicateEncodingsSeparatedByVowels) {
   CHECK_EQUAL(string("J110"), soundex.encode("Jbob")); 
}

TEST(SoundexEncoding, LimitsLengthToFourCharacters) {
   CHECK_EQUAL(4u, soundex.encode("Dcdlb").length());
}

TEST(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
   CHECK_EQUAL(string("I000"), soundex.encode("I"));
}
