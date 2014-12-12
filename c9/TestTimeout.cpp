#include "TestTimeout.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/Utest.h"
#include <iostream>

using namespace std;

TestTimeout::TestTimeout(long limit)
   : TestTimeout(UtestShell::getCurrent()->getName().asCharString(), limit) {}

TestTimeout::TestTimeout(const string& text, long limit) 
   : Start{chrono::system_clock::now()} 
   , Text{text}
   , Limit{limit} {}

TestTimeout::~TestTimeout() {
   Stop = chrono::system_clock::now();
   Elapsed = chrono::duration_cast<chrono::milliseconds>(Stop - Start);
   cout << endl <<
      Text << " elapsed time = " << Elapsed.count() << " ms; time limit = " 
      << Limit.count() << " ms" << endl;
   /* TODO: You are not supposed to throw in the desctructor/
    *       Is there another way of doing this with the RAII idiom?
    *       Then again, if we fail here, the test needs work and the
    *       test suite might as well be terminated.
    */
   CHECK_FALSE (Elapsed.count() > Limit.count() );
}

