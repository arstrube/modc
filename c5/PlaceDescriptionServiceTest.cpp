#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"
#include "PlaceDescriptionService.h"

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
   PlaceDescriptionService* service;
   
   void setup() override {
       service = new PlaceDescriptionService();
   }

   void teardown() override {
       mock().checkExpectations();
       mock().clear();
       delete service;
   }
};

TEST(APlaceDescriptionService, FormatsRetrievedAddressIntoSummaryDescription) {
   auto description = service->summaryDescription(f.ValidLatitude, f.ValidLongitude);

   STRCMP_EQUAL("Drury Ln, Fountain, CO, US", description.c_str());
}
