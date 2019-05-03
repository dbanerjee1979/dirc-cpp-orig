#include <string>
#include <memory>
#include <gtest/gtest.h>
#include <core/irc_server.h>
#include <core/server_event_handler.h>
#include <core/config.h>

namespace core {
  class StubServerEventHandler : public core::ServerEventHandler {
  public:
    StubServerEventHandler() :
      is_connected(false) {
    }
    
    void recieved_message(std::string &msg) {
      msgs.push_back(msg);
    }
    
    void connected() {
      is_connected = true;
    }

    void error(std::string &msg) {
      errors.push_back(msg);
    }

    void notice(std::string &recipient, std::string &msg) {
      notices.push_back(std::pair<std::string, std::string>(recipient, msg));
    }

    void message(std::string &msg) {
      messages.push_back(msg);
    }
    
    bool is_connected;
    std::vector<std::string> msgs;
    std::vector<std::string> errors;
    std::vector<std::pair<std::string, std::string>> notices;
    std::vector<std::string> messages;
  };
  
  class IrcServerTest : public testing::Test {
  protected:
    IrcServerTest() :
      user_info({ "nick", "_nick_", "__nick__" }, "jdoe", "John Doe", ""),
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

  TEST_F(IrcServerTest, fail_to_connect_if_all_nicks_taken) {
    create_server();

    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(msg = ":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(msg = ":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK _nick_\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK __nick__\r", line);
    EXPECT_EQ(false, sh.is_connected);
  }

  TEST_F(IrcServerTest, skip_unparseable_messages) {
    create_server();

    std::string msg;
    server->handle_message(msg = ":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(msg = "~~~~~");
    server->handle_message(msg = ":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(msg = ":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK _nick_\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK __nick__\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

  TEST_F(IrcServerTest, send_notification_messages_to_handler) {
    create_server();

    std::string prefix = ":wolfe.freenode.net NOTICE * :";
    std::string msg1 = "*** Looking up your hostname...";
    std::string msg2 = "*** Checking Ident";
    std::string msg3 = "*** Couldn't look up your hostname";
    std::string msg4 = "*** No Ident response";

    std::string msg;
    server->handle_message(msg = prefix + msg1);
    server->handle_message(msg = prefix + msg2);
    server->handle_message(msg = prefix + msg3);
    server->handle_message(msg = prefix + msg4);

    EXPECT_EQ(4, sh.notices.size());
    if (sh.notices.size() == 4) {
      EXPECT_EQ("*", sh.notices[0].first);
      EXPECT_EQ(msg1, sh.notices[0].second);
      EXPECT_EQ("*", sh.notices[1].first);
      EXPECT_EQ(msg2, sh.notices[1].second);
      EXPECT_EQ("*", sh.notices[2].first);;
      EXPECT_EQ(msg3, sh.notices[2].second);
      EXPECT_EQ("*", sh.notices[3].first);
      EXPECT_EQ(msg4, sh.notices[3].second);
    }
  }

  TEST_F(IrcServerTest, send_messages_to_handler_for_startup_messages) {
    create_server();

    std::string msg;
    server->handle_message(msg = ":wolfe.freenode.net 001 nick :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(msg = ":wolfe.freenode.net 002 nick :Your host is wolfe.freenode.net[2001:948:7:7::140/8001], running version ircd-seven-1.1.7");
    server->handle_message(msg = ":wolfe.freenode.net 003 nick :This server was created Sat Sep 1 2018 at 18:07:18 UTC");
    server->handle_message(msg = ":wolfe.freenode.net 004 nick wolfe.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");
    server->handle_message(msg = ":wolfe.freenode.net 005 nick CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode STATUSMSG=@+ CALLERID=g CASEMAPPING=rfc1459 :are supported by this server");
    server->handle_message(msg = ":wolfe.freenode.net 005 nick CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 DEAF=D FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,ajrxz CLIENTVER=3.0 KNOCK SAFELIST ELIST=CTU :are supported by this server");
    server->handle_message(msg = ":wolfe.freenode.net 005 nick WHOX CPRIVMSG CNOTICE ETRACE :are supported by this server");
    server->handle_message(msg = ":wolfe.freenode.net 251 nick :There are 98 users and 84180 invisible on 32 servers");
    server->handle_message(msg = ":wolfe.freenode.net 252 nick 34 :IRC Operators online");
    server->handle_message(msg = ":wolfe.freenode.net 253 nick 3 :unknown connection(s)");
    server->handle_message(msg = ":wolfe.freenode.net 254 nick 53022 :channels formed");
    server->handle_message(msg = ":wolfe.freenode.net 255 nick :I have 5502 clients and 1 servers");
    server->handle_message(msg = ":wolfe.freenode.net 265 nick 5502 6184 :Current local users 5502, max 6184");
    server->handle_message(msg = ":wolfe.freenode.net 266 nick 84278 94264 :Current global users 84278, max 94264");
    server->handle_message(msg = ":wolfe.freenode.net 250 nick :Highest connection count: 6185 (6184 clients) (735512 connections received)");

    EXPECT_EQ(15, sh.messages.size());
    if (sh.messages.size() == 15) {
      EXPECT_EQ("Welcome to the freenode Internet Relay Chat Network nick", sh.messages[0]);
      EXPECT_EQ("Your host is wolfe.freenode.net[2001:948:7:7::140/8001], running version ircd-seven-1.1.7", sh.messages[1]);
      EXPECT_EQ("This server was created Sat Sep 1 2018 at 18:07:18 UTC", sh.messages[2]);
      EXPECT_EQ("wolfe.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI", sh.messages[3]);
      EXPECT_EQ("CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode STATUSMSG=@+ CALLERID=g CASEMAPPING=rfc1459 are supported by this server", sh.messages[4]);
      EXPECT_EQ("CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 DEAF=D FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,ajrxz CLIENTVER=3.0 KNOCK SAFELIST ELIST=CTU are supported by this server", sh.messages[5]);
      EXPECT_EQ("WHOX CPRIVMSG CNOTICE ETRACE are supported by this server", sh.messages[6]);
      EXPECT_EQ("There are 98 users and 84180 invisible on 32 servers", sh.messages[7]);
      EXPECT_EQ("34 IRC Operators online", sh.messages[8]);
      EXPECT_EQ("3 unknown connection(s)", sh.messages[9]);
      EXPECT_EQ("53022 channels formed", sh.messages[10]);
      EXPECT_EQ("I have 5502 clients and 1 servers", sh.messages[11]);
      EXPECT_EQ("5502 6184 Current local users 5502, max 6184", sh.messages[12]);
      EXPECT_EQ("84278 94264 Current global users 84278, max 94264", sh.messages[13]);
      EXPECT_EQ("Highest connection count: 6185 (6184 clients) (735512 connections received)", sh.messages[14]);
    }
  }
  
}
