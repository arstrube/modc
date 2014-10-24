#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"
#include <string>
#include "CurlHttp.h"

using namespace std;

TEST_GROUP(Http) {};

TEST(Http, WriteCallback) {
   CurlHttp http;
   string buffer("123456789012");
   size_t size(12);
   size_t numberOfMembers(1);
   http.writeCallback("123456789012", size, numberOfMembers, NULL);
   STRCMP_EQUAL("123456789012", http.Response().c_str());
}

TEST_GROUP(HttpIntegration) {};

IGNORE_TEST(HttpIntegration, Get) {
   CurlHttp http;
   http.initialize();
   auto response = http.get("http://langrsoft.com");
   STRCMP_CONTAINS("Jeff Langr", response.c_str());
}

