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
public:
   string returnResponse;
   string expectedURL;
   void initialize() override {}
   std::string get(const std::string& url) const override {
      verify(url);
      return returnResponse;
   }
   void verify(const string& url) const {
      STRCMP_EQUAL(expectedURL.c_str(), url.c_str());
   }
};

TEST(APlaceDescriptionService, ReturnsDescriptionForValidLocation) {
   HttpStub httpStub;
   httpStub.returnResponse = // ...
                             R"({"address": {
                                    "road":"Drury Ln",
                                    "city":"Fountain",
                                    "state":"CO",
                                    "country":"US" }})";
   string urlStart{
      "http://open.mapquestapi.com/nominatim/v1/reverse?format=json&"};
   httpStub.expectedURL = urlStart + 
      "lat=" + PlaceDescriptionServiceFixture::ValidLatitude + "&" +
      "lon=" + PlaceDescriptionServiceFixture::ValidLongitude;
   PlaceDescriptionService service{&httpStub};

   auto description = service.summaryDescription(f.ValidLatitude, f.ValidLongitude);

   STRCMP_EQUAL("Drury Ln, Fountain, CO, US", description.c_str());
}
