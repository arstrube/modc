#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include "PlaceDescriptionService.h"
#include "Http.h"

using namespace std;

class PlaceDescriptionServiceFixture {
public:
   static const string ValidLatitude;
   static const string ValidLongitude;
};

const string PlaceDescriptionServiceFixture::ValidLatitude("38.005");
const string PlaceDescriptionServiceFixture::ValidLongitude("-104.44");

TEST_GROUP(APlaceDescriptionService) {
   PlaceDescriptionServiceFixture f;
};

class HttpStub: public Http {
   void initialize() override {}
   std::string get(const std::string& url) const override {
      verify(url);
      return R"({ "address": {
         "road":"Drury Ln",
         "city":"Fountain",
         "state":"CO",
         "country":"US" }})";
   }
   void verify(const string& url) const {
      auto expectedArgs(
         "lat=" + PlaceDescriptionServiceFixture::ValidLatitude + "&"
         "lon=" + PlaceDescriptionServiceFixture::ValidLongitude);
         STRCMP_CONTAINS(url.c_str(), expectedArgs.c_str());
   }
};

TEST(APlaceDescriptionService, ReturnsDescriptionForValidLocation) {
   HttpStub httpStub;
   PlaceDescriptionService service{&httpStub};

   auto description = service.summaryDescription(f.ValidLatitude, f.ValidLongitude);

   STRCMP_EQUAL("Drury Ln, Fountain, CO, US", description.c_str());
}
