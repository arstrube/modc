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
   string expectedURL;
   void teardown() {
       mock().checkExpectations();
       mock().clear();
   }
};

class HttpStub: public Http {
public:
   void initialize() override {
       mock().actualCall("initialize");
   }
   string get(const string& url) const override {
      mock().actualCall("get").withParameter("url", url.c_str());
      return string(mock().returnValue().getStringValue());
   }
};

TEST(APlaceDescriptionService, HttpIsInitializedUponRequest) {
   HttpStub httpStub;
      
   mock().expectOneCall("initialize");
   mock().expectOneCall("get").ignoreOtherParameters()
         .andReturnValue("");

   PlaceDescriptionService service{&httpStub};

   service.summaryDescription(f.ValidLatitude, f.ValidLongitude);
}

TEST(APlaceDescriptionService, MakesHttpRequestToObtainAddress) {
   HttpStub httpStub;
   string urlStart{"http://open.mapquestapi.com/nominatim/v1/reverse?format=json"};
   expectedURL = urlStart + "&lat=" + f.ValidLatitude + "&lon=" + f.ValidLongitude;
      
   mock().expectOneCall("get").withParameter("url", expectedURL.c_str())
         .andReturnValue("");
   mock().ignoreOtherCalls();

   PlaceDescriptionService service{&httpStub};

   service.summaryDescription(f.ValidLatitude, f.ValidLongitude);
}

TEST(APlaceDescriptionService, FormatsRetrievedAddressIntoSummaryDescription) {
   HttpStub httpStub;
   mock().expectOneCall("get").ignoreOtherParameters()
         .andReturnValue(R"({ "address": {
              "road":"Drury Ln",
              "city":"Fountain",
              "state":"CO",
              "country":"US" }})");
   mock().ignoreOtherCalls();
   
   PlaceDescriptionService service(&httpStub);

   auto description = service.summaryDescription(f.ValidLatitude, f.ValidLongitude);

   STRCMP_EQUAL("Drury Ln, Fountain, CO, US", description.c_str());
}
