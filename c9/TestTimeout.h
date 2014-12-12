/***
 * Excerpted from "Modern C++ Programming with Test-Driven Development",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/lotdd for more book information.
***/
#ifndef TestTimeout_h
#define TestTimeout_h

#include <string>
#include <chrono>

class TestTimeout {
public:
   TestTimeout(long limit);
   TestTimeout(const std::string& text, long limit);
   virtual ~TestTimeout();
private:
   std::chrono::time_point<std::chrono::system_clock> Start;
   std::chrono::time_point<std::chrono::system_clock> Stop;
   std::chrono::milliseconds Elapsed;
   std::string Text;
   std::chrono::milliseconds Limit;
};

#endif
