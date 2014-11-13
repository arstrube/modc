#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "WavReader.h"
#include "Snippet.h"
#include <iostream>
#include <string>
#include <sstream>
#include <memory>

using namespace std;

TEST_GROUP(WavReader) {
};

TEST(WavReader, HasExtension) {
   string s{"a.bcd"};
   CHECK(!hasExtension(s, "xxxx"));
   CHECK(hasExtension(s, "bcd"));
   string bigger{"aaabcd"};
   CHECK(!hasExtension(s, bigger));
}
