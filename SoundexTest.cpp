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
