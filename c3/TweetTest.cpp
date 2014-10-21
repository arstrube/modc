#include "cpputest/testharness.h"
#include "cpputestext/mocksupport.h"
#include "Tweet.h"

TEST_GROUP(ATweet) {};

TEST(ATweet, IsEqualToAnotherWhenMessageAndUserAreEqual) {
   Tweet a("msg", "@user");
   Tweet b("msg", "@user");
   CHECK(a == b);
}

TEST(ATweet, IsNotEqualToAnotherWhenMessagesAreUnequal) {
   Tweet a("msgX", "@user");
   Tweet b("msgY", "@user");
   CHECK(a != b);
}

TEST(ATweet, IsNotEqualToAnotherWhenUsersAreUnequal) {
   Tweet a("msg", "@user1");
   Tweet b("msg", "@user2");
   CHECK(a != b);
}

TEST(ATweet, IsLessThanWhenUserAndMessageAre) {
   Tweet a("msg", "@user1");
   Tweet b("msg", "@user2");
   CHECK(a < b);
}

TEST(ATweet, IsLessThanWhenUserEqualAndMessageIsLessThan) {
   Tweet a("msgA", "@user");
   Tweet b("msgB", "@user");
   CHECK(a < b);
}

TEST(ATweet, CanBeCopyConstructed) {
   Tweet a("msgA", "@user");
   Tweet b(a);
   CHECK(a == b);
}
