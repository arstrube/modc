#include <string>

class Soundex
{
public:
   std::string encode(const std::string& word) const {
      return word + "000";
   }
};

#include "cpputest/testharness.h"

TEST_GROUP(SoundexEncoding)
{
};

TEST(SoundexEncoding, RetainsSoleLetterOfOneLetterWord)
{
   Soundex soundex;

   auto encoded = soundex.encode("A");

   STRCMP_EQUAL("A000", encoded.c_str());
}

TEST(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
   Soundex soundex;

   auto encoded = soundex.encode("I");

   STRCMP_EQUAL("I000", encoded.c_str());
}
