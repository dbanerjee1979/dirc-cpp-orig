#include <string>
#include <memory>
#include <gtest/gtest.h>
#include <core/irc_server.h>
#include <core/server_event_handler.h>
#include <core/config.h>

namespace core {
  class StubServerEventHandler : public core::ServerEventHandler {
  public:
    void recieved_message(std::string &msg) {
      msgs.push_back(msg);
    }
    
    void connected() {
      is_connected = true;
    }

    void error(std::string &msg) {
      errors.push_back(msg);
    }
    
    bool is_connected;
    std::vector<std::string> msgs;
    std::vector<std::string> errors;
  };
  
  class IrcServerTest : public testing::Test {
  protected:
    IrcServerTest() :
      user_info({ "nick", "_nick_" }, "jdoe", "John Doe", ""),
      network("Freenode", { config::Server("irc.freenode.net", 8001) }, user_info),
      user_info_pass({ "nick" }, "jdoe", "John Doe", "secret"),
      network_pass("Freenode", { config::Server("irc.freenode.net", 8001) }, user_info_pass) {
    }

    ~IrcServerTest() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    void create_server() {
      create_server(network);
    }

    void create_server(config::Network &_network) {
      server = std::unique_ptr<IrcServer>(new IrcServer(_network, ss, sh));
    }

    config::UserInfo user_info;
    config::Network network;
    config::UserInfo user_info_pass;
    config::Network network_pass;
    std::stringstream ss;
    StubServerEventHandler sh;
    std::unique_ptr<IrcServer> server;
  };

  TEST_F(IrcServerTest, test_logging_messages) {
    create_server();
    
    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(msg = ":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server->handle_message(msg = ":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server->handle_message(msg = ":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    EXPECT_EQ(4, sh.msgs.size());
    if (sh.msgs.size() == 4) {
      EXPECT_EQ(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick", sh.msgs[0]);
      EXPECT_EQ(":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7", sh.msgs[1]);
      EXPECT_EQ(":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC", sh.msgs[2]);
      EXPECT_EQ(":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI", sh.msgs[3]);
    }
  }
  
  TEST_F(IrcServerTest, test_send_nick_and_user_info_for_connection_registration) {
    create_server();

    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(msg = ":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server->handle_message(msg = ":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server->handle_message(msg = ":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

  TEST_F(IrcServerTest, send_pass_and_nick_and_user_info_for_connection_registration) {
    create_server(network_pass);

    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(msg = ":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server->handle_message(msg = ":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server->handle_message(msg = ":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("PASSWORD secret\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

  TEST_F(IrcServerTest, send_second_nick_if_first_nick_is_already_taken) {
    create_server();

    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK _nick_\r", line);


    EXPECT_EQ(1, sh.errors.size());
    if (sh.errors.size() == 1) {
      EXPECT_EQ("Nickname is already in use.", sh.errors[0]);
    }

    EXPECT_EQ(true, sh.is_connected);
  }

  TEST_F(IrcServerTest, send_second_nick_if_first_nick_collides) {
    create_server();

    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 436 * duke :Nickname collision KILL from");
    server->handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK _nick_\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

}
