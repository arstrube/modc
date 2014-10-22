#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

/** CppUTest does not directly support this.
 *  Simulate it by using a loop & macros inside TEST()
 */
 
class Calculator {
public:
   static int add(int a, int b) {
      return a + b;
   }
   static int multiply(int a, int b) {
      return a * b;
   }
};

struct TestCase {
   int a, b, expected;
   TestCase(int anA, int aB, int anExpected) 
      : a(anA), b(aB), expected(anExpected) {}
};

TEST_GROUP(ACalculator) {
};

TEST(ACalculator, AddsTwoNumbers) {
   LONGS_EQUAL(2, Calculator::add(1, 1));
}

TEST(ACalculator, MultipliesTwoNumbers) {
   LONGS_EQUAL(6, Calculator::multiply(2, 3));
}

#define CHECK_TABLE(object, method, table, element) {\
   for(auto i=0u; i < sizeof(table) / sizeof(element); i++) {\
      LONGS_EQUAL(table[i].expected, object::method(table[i].a, table[i].b));\
   }\
}

TestCase addition_table[] = { 
   TestCase( 1, 1, 2), 
   TestCase( 1, 2, 3),
   TestCase( 2, 2, 4),
   TestCase(-2, 2, 0),
   TestCase(-2,-2,-4)
};

TestCase multiplication_table[] = { 
   TestCase( 1, 1, 1), 
   TestCase( 1, 2, 2),
   TestCase( 2, 2, 4),
   TestCase(-2, 2,-4),
   TestCase(-2,-2, 4)
};

TEST(ACalculator, AddsTwoNumbersLotsOfTimes) {
   CHECK_TABLE(Calculator, add, addition_table, TestCase);
}

TEST(ACalculator, MultipliesTwoNumbersLotsOfTimes) {
   CHECK_TABLE(Calculator, multiply, multiplication_table, TestCase);
}
