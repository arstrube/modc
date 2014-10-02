class Soundex {
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

