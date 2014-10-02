class Soundex {
};

#include "cpputest/testharness.h"

TEST_GROUP(SoundexEncoding)
{
};

TEST(SoundexEncoding, RetainsSoleLetterOfOneLetterWord)
{
   Soundex soundex;
   (void)soundex;  // -Werror=unused-variable
}

