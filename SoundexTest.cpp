#include <string>

class Soundex
{
public:
   std::string encode(const std::string&) const {
      return "A";
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

   STRCMP_EQUAL("A", encoded.c_str());
}

