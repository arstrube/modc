#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"
#include "AddressExtractor.h"

#include <algorithm>
#include <string>

using namespace std;

TEST_GROUP(AnAddressExtractor) {
public:
   AddressExtractor extractor;
   bool IsEmpty(Address arg) {
      return 
         arg.road.empty() &&
         arg.city.empty() &&
         arg.state.empty() &&
         arg.country.empty();
   }
};


TEST(AnAddressExtractor, ReturnsAnEmptyAddressOnAFailedParse) {
   auto address = extractor.addressFrom("not valid json");

   CHECK(IsEmpty(address));
}

TEST(AnAddressExtractor, ReturnsAnEmptyAddressWhenNoAddressFound) {
   auto json = R"({ "place_id":"15331615" })";

   auto address = extractor.addressFrom(json);

   CHECK(IsEmpty(address));
}

TEST(AnAddressExtractor, ReturnsPopulatedAddressForValidJsonResult) {
   auto json = R"({
         "place_id":"15331615",
         "address":{
            "road":"War Eagle Court",
            "city":"Colorado Springs",
            "state":"Colorado",
            "country":"United States of America",
         }
      })";

   auto address = extractor.addressFrom(json);

   STRCMP_EQUAL("War Eagle Court", address.road.c_str());
   STRCMP_EQUAL("Colorado Springs", address.city.c_str());
   STRCMP_EQUAL("Colorado", address.state.c_str());
   STRCMP_EQUAL("United States of America", address.country.c_str());
}

TEST(AnAddressExtractor, DefaultsNonexistentFieldsToEmpty) {
   auto json = R"({
         "address":{
            "road":"War Eagle Court",
            "city":"Colorado Springs",
            "country":"United States of America" }})";

   auto address = extractor.addressFrom(json);

   STRCMP_EQUAL("", address.state.c_str());
}

