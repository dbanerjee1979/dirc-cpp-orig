#include <string>
#include <gtest/gtest.h>
#include <core/irc_server.h>
#include <core/server_event_handler.h>

namespace core {
  class StubServerEventHandler : public core::ServerEventHandler {
  public:
    bool is_connected;
  };
  
  class IrcServerTest : public testing::Test {
  protected:
    IrcServerTest() :
      server(ss, sh) {
    }

    ~IrcServerTest() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    std::stringstream ss;
    StubServerEventHandler sh;
    IrcServer server;
  };
  
  TEST_F(IrcServerTest, send_nick_and_user_info_for_connection_registration) {
    std::string msg;
    server.handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server.handle_message(msg = ":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server.handle_message(msg = ":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server.handle_message(msg = ":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER foo 8 * :John\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

}
