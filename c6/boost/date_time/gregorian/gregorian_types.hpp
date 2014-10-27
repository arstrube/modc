#ifndef DATE_TIME_DATE_HPP___
#define DATE_TIME_DATE_HPP___

namespace boost {
namespace gregorian {

/** Stub for class boost::gregorian::date
  *
  */

enum Month {Jan=1,Feb,Mar};

class date
{
public:

   date(unsigned y, Month m, unsigned d) : year_(y), month_(m), day_(d) {}

   bool operator==(const date& rhs) const
   {
      return day_ == rhs.day_ && month_ == rhs.month_ && year_ == rhs.year_;
   }
   
private:
   unsigned year_;
   Month month_;
   unsigned day_;
};

} } // namespaces

#endif
