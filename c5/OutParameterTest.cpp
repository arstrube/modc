#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

class DifficultCollaborator {
public:
   virtual bool calculate(int* /*result*/) 
   {
      throw 1;
   }
};

class Target {
public:
   int execute(DifficultCollaborator* calculator) {
      int i;
      if (!calculator->calculate(&i))
         return 0;
      return i;
   }
};

class DifficultCollaboratorMock: public DifficultCollaborator {
public:
   virtual bool calculate(int* result) override {
      mock().actualCall("calculate").withOutputParameter("result", result);
      return mock().returnValue().getUnsignedIntValue();
   }
};

TEST_GROUP(ATarget) {};

TEST(ATarget, ReturnsAnAmountWhenCalculatePasses) {
   DifficultCollaboratorMock difficult;
   Target calc;
   const int value = 3;
   mock().expectOneCall("calculate")
         .withOutputParameterReturning("result", &value, sizeof(value))
         .andReturnValue(true);

   auto result = calc.execute(&difficult);
   
   LONGS_EQUAL(3, result);
   mock().checkExpectations();
   mock().clear();
}
