#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"
#include <memory>
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
   string expectedURL;
   shared_ptr<HttpStub> httpStub{new HttpStub};
   void teardown() {
       mock().checkExpectations();
       mock().clear();
   }
};

class PlaceDescriptionService_StubHttpService: public PlaceDescriptionService {
public:
   PlaceDescriptionService_StubHttpService(shared_ptr<HttpStub> httpStub) 
      : httpStub_{httpStub} {}
   shared_ptr<Http> httpService() const override { return httpStub_; }
   shared_ptr<Http> httpStub_;
};

TEST(APlaceDescriptionService, MakesHttpRequestToObtainAddress) {
   string urlStart{"http://open.mapquestapi.com/nominatim/v1/reverse?format=json"};
   expectedURL = urlStart + "&lat=" + f.ValidLatitude + "&lon=" + f.ValidLongitude;
      
   mock().expectOneCall("get").withParameter("url", expectedURL.c_str())
         .andReturnValue("");
   mock().ignoreOtherCalls();

   PlaceDescriptionService_StubHttpService service{httpStub};

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
   
   PlaceDescriptionService_StubHttpService service{httpStub};

   auto description = service.summaryDescription(f.ValidLatitude, f.ValidLongitude);

   STRCMP_EQUAL("Drury Ln, Fountain, CO, US", description.c_str());
}

TEST(APlaceDescriptionService, HttpIsInitializedUponConstruction) {   
   mock().strictOrder();
   mock().expectOneCall("initialize");
   mock().expectOneCall("get").ignoreOtherParameters()
         .andReturnValue("");

   PlaceDescriptionService_StubHttpService service{httpStub};

   service.summaryDescription(f.ValidLatitude, f.ValidLongitude);
}
