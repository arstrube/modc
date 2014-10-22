#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

/** CppUTest does not directly support this.
 *  Simulate it by using a loop & macros inside TEST()
 */
 
class Adder {
public:
   static int sum(int a, int b) {
      return a + b;
   }
};

struct SumCase {
   int a, b, expected;
   SumCase(int anA, int aB, int anExpected) 
      : a(anA), b(aB), expected(anExpected) {}
};

TEST_GROUP(AnAdder) {
};

TEST(AnAdder, GeneratesASumFromTwoNumbers) {
   LONGS_EQUAL(2, Adder::sum(1, 1));
}

#define CHECK_TABLE(object, method, table, element) {\
   for(auto i=0u; i < sizeof(table) / sizeof(element); i++) {\
      LONGS_EQUAL(table[i].expected, object::method(table[i].a, table[i].b));\
   }\
}

SumCase sums[] = { 
   SumCase( 1, 1, 2), 
   SumCase( 1, 2, 3),
   SumCase( 2, 2, 4),
   SumCase(-2, 2, 0),
   SumCase(-2,-2,-4)
};

TEST(AnAdder, GeneratesLotsOfSumsFromTwoNumbers) {
   CHECK_TABLE(Adder, sum, sums, SumCase);
}
