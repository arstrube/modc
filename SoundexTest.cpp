#include <string>

class Soundex
{
public:
   std::string encode(const std::string&) const {
      return "";
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
}

