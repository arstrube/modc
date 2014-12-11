#include "CppUTest/TestHarness.h"

#include "Soundex.h"

using namespace std;

TEST_GROUP(SoundexEncoding) {};

TEST(SoundexEncoding, AppendsZerosToWordForOneLetterWord) {
   Soundex soundex;
   auto encoded = soundex.encode("A");

   CHECK_EQUAL(string("A000"), encoded);
}
