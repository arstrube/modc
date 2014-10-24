#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"

#include "PlaceDescriptionService.h"
#include "Http.h"

using namespace std;

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

class PlaceDescriptionServiceFixture {
public:
   static const string ValidLatitude;
   static const string ValidLongitude;
};
const string PlaceDescriptionServiceFixture::ValidLatitude("38.005");
const string PlaceDescriptionServiceFixture::ValidLongitude("-104.44");

TEST_GROUP(APlaceDescriptionService) {
   PlaceDescriptionServiceFixture f;
   PlaceDescriptionServiceTemplate<HttpStub> service;
   string expectedURL;
   
   void teardown() override {
       mock().checkExpectations();
       mock().clear();
   }
};

TEST(APlaceDescriptionService, MakesHttpRequestToObtainAddress) {
   string urlStart{"http://open.mapquestapi.com/nominatim/v1/reverse?format=json"};
   expectedURL = urlStart + "&lat=" + f.ValidLatitude + "&lon=" + f.ValidLongitude;
      
   mock().expectOneCall("get").withParameter("url", expectedURL.c_str())
         .andReturnValue("");
   mock().ignoreOtherCalls();

   service.summaryDescription(f.ValidLatitude, f.ValidLongitude);
}

TEST(APlaceDescriptionService, FormatsRetrievedAddressIntoSummaryDescription) {
   mock().expectOneCall("get").ignoreOtherParameters()
         .andReturnValue(R"({ "address": {
              "road":"Drury Ln",
              "city":"Fountain",
              "state":"CO",
              "country":"US" }})");
   mock().ignoreOtherCalls();
   
   auto description = service.summaryDescription(f.ValidLatitude, f.ValidLongitude);

   STRCMP_EQUAL("Drury Ln, Fountain, CO, US", description.c_str());
}

TEST(APlaceDescriptionService, HttpIsInitializedUponConstruction) {   
   mock().strictOrder();
   mock().expectOneCall("initialize");
   mock().expectOneCall("get").ignoreOtherParameters()
         .andReturnValue("");

   service.summaryDescription(f.ValidLatitude, f.ValidLongitude);
}
