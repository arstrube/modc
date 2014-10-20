#include "Soundex.h"
#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

TEST_GROUP(SoundexEncoding)
{
   Soundex soundex;
};

TEST(SoundexEncoding, RetainsSoleLetterOfOneLetterWord) {
   STRCMP_EQUAL("A000", soundex.encode("A").c_str());
}

TEST(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
   STRCMP_EQUAL("I000", soundex.encode("I").c_str());
}

TEST(SoundexEncoding, ReplacesConsonantsWithAppropriateDigits) {
   STRCMP_EQUAL("A200", soundex.encode("Ax").c_str());
}

TEST(SoundexEncoding, IgnoresNonAlphabetics) {
   STRCMP_EQUAL("A000", soundex.encode("A$").c_str());
}

TEST(SoundexEncoding, ReplacesMultipleConsonantsWithDigits) {
   STRCMP_EQUAL("A234", soundex.encode("Acdl").c_str());
}

TEST(SoundexEncoding, LimitsLengthToFourCharacters) {
   LONGS_EQUAL(4u, soundex.encode("Dcdlb").length());
}

TEST(SoundexEncoding, IgnoresVowelLikeLetters) {
   STRCMP_EQUAL("B234", soundex.encode("Baeiouhycdl").c_str());
}

TEST(SoundexEncoding, CombinesDuplicateEncodings) {
   STRCMP_EQUAL(soundex.encodedDigit('b').c_str(), soundex.encodedDigit('f').c_str());
   STRCMP_EQUAL(soundex.encodedDigit('c').c_str(), soundex.encodedDigit('g').c_str());
   STRCMP_EQUAL(soundex.encodedDigit('d').c_str(), soundex.encodedDigit('t').c_str());
   STRCMP_EQUAL("A123", soundex.encode("Abfcgdt").c_str());
}

TEST(SoundexEncoding, UpperCasesFirstLetter) {
   STRNCMP_EQUAL("A", soundex.encode("abcd").c_str(), 1);
}
