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
}
