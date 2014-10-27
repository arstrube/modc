#ifndef DATE_TIME_DATE_HPP___
#define DATE_TIME_DATE_HPP___

namespace boost {
namespace gregorian {

/** Stub for class boost::gregorian::date
  *
  */

enum Month {Jan=1};

class date
{
public:

   date(int /*y*/, Month /*m*/, int /*d*/) /* : years_(y), months(m), days_(d) */ {}

   bool operator==(const date& /*rhs*/) const
   {
      return false;
   }
};

} } // namespaces

#endif
