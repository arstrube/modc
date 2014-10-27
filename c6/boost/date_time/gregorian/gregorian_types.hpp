#ifndef DATE_TIME_DATE_HPP___
#define DATE_TIME_DATE_HPP___

namespace boost {
namespace gregorian {

/** Header stub for class boost::gregorian::date */

enum Month {Jan=1,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec};

class date {
public:
   date(unsigned y, Month m, unsigned d) : y_(y), m_(m), d_(d) {}
   bool operator==(const date& rhs) const {
      return d_ == rhs.d_ && m_ == rhs.m_ && y_ == rhs.y_;
   }
private:
   unsigned y_, m_, d_;
};

} } // namespaces

#endif
