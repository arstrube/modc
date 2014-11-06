#ifndef DATE_TIME_DATE_HPP___
#define DATE_TIME_DATE_HPP___

namespace boost {
namespace gregorian {

/** Header stub for class boost::gregorian::date */

#define ARBITRARY_DATE date(2014, Mar, 15)

enum Month {Jan=1,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec};

class date_duration {
public:
   explicit date_duration(int day_count = 0) : d_(day_count) {}
   unsigned get_days() const { return d_; }
private:
   unsigned d_;
};

class date {
public:
   date(unsigned y, Month m, unsigned d) : y_(y), m_(m), d_(d) {}
   date() {
      d_ = -1;
   }
   bool operator==(const date& rhs) const {
      return d_ == rhs.d_ && m_ == rhs.m_ && y_ == rhs.y_;
   }
   date operator+(const date& rhs) const {
      return date(y_, m_, d_ + rhs.d_);
   }
   date operator+(const date_duration& rhs) const {
      return date(y_, m_, d_ + rhs.get_days());
   }
   bool operator>(const date& rhs) const {
      return d_ > rhs.d_;
   }
   bool is_not_a_date() {
       return d_ < 0;
   }
private:
   unsigned y_;
   Month m_;
   int d_;
};

class day_clock {
public:
  static date local_day() { return ARBITRARY_DATE; };
  static date universal_day() { return ARBITRARY_DATE; }
};


} } // namespaces

#endif
