#include "CppUTest/TestHarness.h"
#include "RomanConverter.h"

TEST_GROUP(ARomanConverter) {};

TEST(ARomanConverter, AnswersArabicEquivalents)
{
   RomanConverter converter;
   STRCMP_EQUAL("I",         converter.convert(1).c_str());
   STRCMP_EQUAL("II",        converter.convert(2).c_str());
   STRCMP_EQUAL("III",       converter.convert(3).c_str());
   STRCMP_EQUAL("IV",        converter.convert(4).c_str());
   STRCMP_EQUAL("V",         converter.convert(5).c_str());
   STRCMP_EQUAL("VI",        converter.convert(6).c_str());
   STRCMP_EQUAL("VII",       converter.convert(7).c_str());
   STRCMP_EQUAL("VIII",      converter.convert(8).c_str());
   STRCMP_EQUAL("IX",        converter.convert(9).c_str());
   STRCMP_EQUAL("X",         converter.convert(10).c_str());
   STRCMP_EQUAL("XIV",       converter.convert(14).c_str());
   STRCMP_EQUAL("XIX",       converter.convert(19).c_str());
   STRCMP_EQUAL("XL",        converter.convert(40).c_str());
   STRCMP_EQUAL("XC",        converter.convert(90).c_str());
   STRCMP_EQUAL("CD",        converter.convert(400).c_str());
   STRCMP_EQUAL("CM",        converter.convert(900).c_str());
   STRCMP_EQUAL("L",         converter.convert(50).c_str());
   STRCMP_EQUAL("C",         converter.convert(100).c_str());
   STRCMP_EQUAL("D",         converter.convert(500).c_str());
   STRCMP_EQUAL("M",         converter.convert(1000).c_str());
   STRCMP_EQUAL("MCMLXXXIX", converter.convert(1989).c_str());
  // ...
}
