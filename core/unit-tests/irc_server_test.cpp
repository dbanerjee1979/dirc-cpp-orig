#include <string>
#include <memory>
#include <gtest/gtest.h>
#include <core/irc_server.h>
#include <core/server_event_handler.h>
#include <core/channel_event_handler.h>
#include <core/chat_event_handler.h>
#include <core/config.h>

namespace core {
  class StubChannelEventHandlerST : public core::ChannelEventHandler {
  public:
    StubChannelEventHandlerST(const std::string& _name) :
      name(_name) {
    }

    void topic_changed(const std::string& msg) {
      topic = msg;
    }

    void user_quit(IrcChannelUser &_user, const std::string &msg) {
      quit_user = _user.user().nickname();
      quit_message = msg;
    }

    void nick_changed(const std::string &_nick_from, const std::string &_nick_to) {
      nick_from = _nick_from;
      nick_to = _nick_to;
    }
    
    std::string name;
    std::string topic;
    boost::optional<std::string> quit_user;
    boost::optional<std::string> quit_message;
    boost::optional<std::string> nick_from;
    boost::optional<std::string> nick_to;
  };

  class StubChatEventHandlerST : public core::ChatEventHandler {
  public:
    void recieved_message(const std::string &msg) {
      private_msgs.push_back(msg);
    }

    std::vector<std::string> private_msgs;
  };
  
  class StubServerEventHandler : public core::ServerEventHandler {
  public:
    StubServerEventHandler() :
      is_connected(false),
      is_shutdown(false) {
    }
    
    void recieved_message(const std::string &msg) {
      msgs.push_back(msg);
    }
    
    void connected() {
      is_connected = true;
    }

    void handle_shutdown() {
      is_shutdown = true;
    }

    std::shared_ptr<core::ChannelEventHandler> create_channel_event_handler(const std::string &channel) {
      std::shared_ptr<StubChannelEventHandlerST> ch(new StubChannelEventHandlerST(channel));
      channels.push_back(ch);
      return ch;
    }

    std::shared_ptr<core::ChatEventHandler> create_private_chat_event_handler(const IrcUser &user) {
      std::shared_ptr<StubChatEventHandlerST> pch(new StubChatEventHandlerST());
      private_chats.push_back(pch);
      return pch;
    }

    void error(const std::string &msg) {
      errors.push_back(msg);
    }

    void notice(const std::string &recipient, const std::string &msg) {
      notices.push_back(std::pair<std::string, std::string>(recipient, msg));
    }

    void message(const std::string &msg) {
      messages.push_back(msg);
    }

    void message_of_the_day(const std::string &msg) {
      motd = msg;
    }

    void user_quit(IrcUser &_user, const std::string &msg) {
      quit_user = _user.nickname();
      quit_message = msg;
    }

    void nick_changed(const std::string &_nick_from, const std::string &_nick_to) {
      nick_from = _nick_from;
      nick_to = _nick_to;
    }
    
    bool is_connected;
    std::vector<std::string> msgs;
    std::vector<std::string> errors;
    std::vector<std::pair<std::string, std::string>> notices;
    std::vector<std::string> messages;
    std::string motd;
    bool is_shutdown;
    std::vector<std::shared_ptr<StubChannelEventHandlerST>> channels;
    std::vector<std::shared_ptr<StubChatEventHandlerST>> private_chats;
    boost::optional<std::string> quit_user;
    boost::optional<std::string> quit_message;
    boost::optional<std::string> nick_from;
    boost::optional<std::string> nick_to;
  };
  
  class IrcServerTest : public testing::Test {
  protected:
    IrcServerTest() :
      user_info({ "nick", "_nick_", "__nick__" }, "jdoe", "John Doe", ""),
      network("Freenode", { config::Server("irc.freenode.net", 8001) }, user_info),
      user_info_pass({ "nick" }, "jdoe", "John Doe", "secret"),
      network_pass("Freenode", { config::Server("irc.freenode.net", 8001) }, user_info_pass),
      entity_repo(sh, chat_factory) {
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
      server = std::unique_ptr<IrcServer>(new IrcServer(_network, ss, sh, entity_repo));
    }

    config::UserInfo user_info;
    config::Network network;
    config::UserInfo user_info_pass;
    config::Network network_pass;
    std::stringstream ss;
    StubServerEventHandler sh;
    std::unique_ptr<IrcServer> server;
    ChatEventHandlerFactory chat_factory;
    IrcEntityRepository entity_repo;
  };

  TEST_F(IrcServerTest, should_log_messages_received_from_server) {
    create_server();
    
    server->handle_message(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server->handle_message(":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server->handle_message(":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    EXPECT_EQ(4, sh.msgs.size());
    if (sh.msgs.size() == 4) {
      EXPECT_EQ(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick", sh.msgs[0]);
      EXPECT_EQ(":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7", sh.msgs[1]);
      EXPECT_EQ(":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC", sh.msgs[2]);
      EXPECT_EQ(":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI", sh.msgs[3]);
    }
  }
  
  TEST_F(IrcServerTest, should_send_nick_and_user_info_for_connection_registration) {
    create_server();

    server->handle_message(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server->handle_message(":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server->handle_message(":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    EXPECT_EQ(true, sh.is_connected);

    auto user = entity_repo.find_user("nick");
    EXPECT_EQ(true, (bool) user);
    if (user) {
      EXPECT_EQ("nick", user->nickname());
      EXPECT_EQ("jdoe", user->username());
      EXPECT_EQ("John Doe", user->realname());
    }
  }

  TEST_F(IrcServerTest, should_send_pass_and_nick_and_user_info_for_connection_registration) {
    create_server(network_pass);

    server->handle_message(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":rajaniemi.freenode.net 002 shorugoru :Your host is rajaniemi.freenode.net[2001:708:40:2001::f5ee:d0de/8001], running version ircd-seven-1.1.7");
    server->handle_message(":rajaniemi.freenode.net 003 shorugoru :This server was created Tue Sep 25 2018 at 13:21:15 UTC");
    server->handle_message(":rajaniemi.freenode.net 004 shorugoru rajaniemi.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("PASSWORD secret\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

  TEST_F(IrcServerTest, should_send_second_nick_if_first_nick_is_already_taken) {
    create_server();

    server->handle_message(":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");

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

    EXPECT_EQ(false, (bool) entity_repo.find_user("nick"));
    EXPECT_EQ(true, (bool) entity_repo.find_user("_nick_"));
  }

  TEST_F(IrcServerTest, should_send_second_nick_if_first_nick_collides) {
    create_server();

    server->handle_message(":rajaniemi.freenode.net 436 * duke :Nickname collision KILL from");
    server->handle_message(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick\r", line);
    getline(ss, line);
    EXPECT_EQ("USER jdoe 8 * :John Doe\r", line);
    getline(ss, line);
    EXPECT_EQ("NICK _nick_\r", line);
    EXPECT_EQ(true, sh.is_connected);
  }

  TEST_F(IrcServerTest, should_fail_to_connect_if_all_nicks_taken) {
    create_server();

    server->handle_message(":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");

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

  TEST_F(IrcServerTest, should_ignore_unparseable_messages) {
    create_server();

    server->handle_message(":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message("~~~~~");
    server->handle_message(":rajaniemi.freenode.net 433 * duke :Nickname is already in use.");
    server->handle_message(":rajaniemi.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");

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

  TEST_F(IrcServerTest, should_handle_notification_messages_from_server) {
    create_server();

    std::string prefix = ":wolfe.freenode.net NOTICE * :";
    std::string msg1 = "*** Looking up your hostname...";
    std::string msg2 = "*** Checking Ident";
    std::string msg3 = "*** Couldn't look up your hostname";
    std::string msg4 = "*** No Ident response";

    server->handle_message(prefix + msg1);
    server->handle_message(prefix + msg2);
    server->handle_message(prefix + msg3);
    server->handle_message(prefix + msg4);

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

  TEST_F(IrcServerTest, should_handle_startup_messages_from_server) {
    create_server();

    server->handle_message(":wolfe.freenode.net 001 nick :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":wolfe.freenode.net 002 nick :Your host is wolfe.freenode.net[2001:948:7:7::140/8001], running version ircd-seven-1.1.7");
    server->handle_message(":wolfe.freenode.net 003 nick :This server was created Sat Sep 1 2018 at 18:07:18 UTC");
    server->handle_message(":wolfe.freenode.net 004 nick wolfe.freenode.net ircd-seven-1.1.7 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI");
    server->handle_message(":wolfe.freenode.net 005 nick CHANTYPES=# EXCEPTS INVEX CHANMODES=eIbq,k,flj,CFLMPQScgimnprstz CHANLIMIT=#:120 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=freenode STATUSMSG=@+ CALLERID=g CASEMAPPING=rfc1459 :are supported by this server");
    server->handle_message(":wolfe.freenode.net 005 nick CHARSET=ascii NICKLEN=16 CHANNELLEN=50 TOPICLEN=390 DEAF=D FNC TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,ajrxz CLIENTVER=3.0 KNOCK SAFELIST ELIST=CTU :are supported by this server");
    server->handle_message(":wolfe.freenode.net 005 nick WHOX CPRIVMSG CNOTICE ETRACE :are supported by this server");
    server->handle_message(":wolfe.freenode.net 251 nick :There are 98 users and 84180 invisible on 32 servers");
    server->handle_message(":wolfe.freenode.net 252 nick 34 :IRC Operators online");
    server->handle_message(":wolfe.freenode.net 253 nick 3 :unknown connection(s)");
    server->handle_message(":wolfe.freenode.net 254 nick 53022 :channels formed");
    server->handle_message(":wolfe.freenode.net 255 nick :I have 5502 clients and 1 servers");
    server->handle_message(":wolfe.freenode.net 265 nick 5502 6184 :Current local users 5502, max 6184");
    server->handle_message(":wolfe.freenode.net 266 nick 84278 94264 :Current global users 84278, max 94264");
    server->handle_message(":wolfe.freenode.net 250 nick :Highest connection count: 6185 (6184 clients) (735512 connections received)");

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

  TEST_F(IrcServerTest, should_handle_message_of_the_day) {
    create_server();

    server->handle_message(":wolfe.freenode.net 375 nick :- wolfe.freenode.net Message of the Day -");
    server->handle_message(":wolfe.freenode.net 372 nick :- Welcome to wolfe.freenode.net in Stockholm, SE.");
    server->handle_message(":wolfe.freenode.net 372 nick :- Thanks to http://nordu.net/ for sponsoring");
    server->handle_message(":wolfe.freenode.net 372 nick :- this server!");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- WOLFE, GENE [1931-2019].  Prolific writer of short stories");
    server->handle_message(":wolfe.freenode.net 372 nick :- and novels. His best-known work is the multi-volume novel The");
    server->handle_message(":wolfe.freenode.net 372 nick :- Book of the New Sun. He has won multiple awards including");
    server->handle_message(":wolfe.freenode.net 372 nick :- the Nebula Award, the World Fantasy Award, The Campell");
    server->handle_message(":wolfe.freenode.net 372 nick :- Memorial Award and the Locus Award. He was awarded the World");
    server->handle_message(":wolfe.freenode.net 372 nick :- Fantasy Award for lifetime achievement in 1996.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- Welcome to freenode - supporting the free and open source");
    server->handle_message(":wolfe.freenode.net 372 nick :- software communities since 1998.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- By connecting to freenode you indicate that you have read and");
    server->handle_message(":wolfe.freenode.net 372 nick :- accept our policies and guidelines as set out on https://freenode.net");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- In the event that you observe behaviour that contravenes our policies,");
    server->handle_message(":wolfe.freenode.net 372 nick :- please notify a volunteer staff member via private message, or send us an");
    server->handle_message(":wolfe.freenode.net 372 nick :- e-mail to complaints@freenode.net -- we will do our best to address the");
    server->handle_message(":wolfe.freenode.net 372 nick :- situation within a reasonable period of time, and we may request further");
    server->handle_message(":wolfe.freenode.net 372 nick :- information or, as appropriate, involve other parties such as channel operators");
    server->handle_message(":wolfe.freenode.net 372 nick :- Group Contacts representing an on-topic group.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- freenode runs an open proxy scanner.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- If you are looking for assistance, you may be able to find a list of");
    server->handle_message(":wolfe.freenode.net 372 nick :- volunteer staff on '/stats p' (shows only on-call staff) or by joining");
    server->handle_message(":wolfe.freenode.net 372 nick :- #freenode and using the '/who freenode/staff/*' command. You may message");
    server->handle_message(":wolfe.freenode.net 372 nick :- any of us at any time. Please note that freenode predominantly provides");
    server->handle_message(":wolfe.freenode.net 372 nick :- assistance via private message, and while we have a network channel the");
    server->handle_message(":wolfe.freenode.net 372 nick :- primary venue for support requests is via private message to a member");
    server->handle_message(":wolfe.freenode.net 372 nick :- of the volunteer staff team.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- From time to time, volunteer staff may send server-wide notices relating to");
    server->handle_message(":wolfe.freenode.net 372 nick :- the project, or the communities that we host. The majority of such notices");
    server->handle_message(":wolfe.freenode.net 372 nick :- will be sent as wallops, and you can '/mode <yournick> +w' to ensure that you");
    server->handle_message(":wolfe.freenode.net 372 nick :- do not miss them. Important messages relating to the freenode project, including");
    server->handle_message(":wolfe.freenode.net 372 nick :- notices of upcoming maintenance and other scheduled downtime will be issued as");
    server->handle_message(":wolfe.freenode.net 372 nick :- global notices.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- Representing an on-topic project? Don't forget to register, more information");
    server->handle_message(":wolfe.freenode.net 372 nick :- can be found on the https://freenode.net website under \"Group Registration\".");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- Thank you also to our server sponsors for the sustained support in keeping the");
    server->handle_message(":wolfe.freenode.net 372 nick :- network going for close to two decades.");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- Thank you to all of our attendees, sponsors, speakers, exhibitors, helpers,");
    server->handle_message(":wolfe.freenode.net 372 nick :- and everyone else who made this year's freenode #live conference amazing.");
    server->handle_message(":wolfe.freenode.net 372 nick :- https://freenode.net/news/live-2018");
    server->handle_message(":wolfe.freenode.net 372 nick :-");
    server->handle_message(":wolfe.freenode.net 372 nick :- Thank you for using freenode!");
    server->handle_message(":wolfe.freenode.net 376 nick :End of /MOTD command.");

    std::stringstream motd;
    motd
      << "- wolfe.freenode.net Message of the Day -\n"
      << "- Welcome to wolfe.freenode.net in Stockholm, SE.\n"
      << "- Thanks to http://nordu.net/ for sponsoring\n"
      << "- this server!\n"
      << "-\n"
      << "- WOLFE, GENE [1931-2019].  Prolific writer of short stories\n"
      << "- and novels. His best-known work is the multi-volume novel The\n"
      << "- Book of the New Sun. He has won multiple awards including\n"
      << "- the Nebula Award, the World Fantasy Award, The Campell\n"
      << "- Memorial Award and the Locus Award. He was awarded the World\n"
      << "- Fantasy Award for lifetime achievement in 1996.\n"
      << "-\n"
      << "- Welcome to freenode - supporting the free and open source\n"
      << "- software communities since 1998.\n"
      << "-\n"
      << "- By connecting to freenode you indicate that you have read and\n"
      << "- accept our policies and guidelines as set out on https://freenode.net\n"
      << "-\n"
      << "- In the event that you observe behaviour that contravenes our policies,\n"
      << "- please notify a volunteer staff member via private message, or send us an\n"
      << "- e-mail to complaints@freenode.net -- we will do our best to address the\n"
      << "- situation within a reasonable period of time, and we may request further\n"
      << "- information or, as appropriate, involve other parties such as channel operators\n"
      << "- Group Contacts representing an on-topic group.\n"
      << "-\n"
      << "- freenode runs an open proxy scanner.\n"
      << "-\n"
      << "- If you are looking for assistance, you may be able to find a list of\n"
      << "- volunteer staff on '/stats p' (shows only on-call staff) or by joining\n"
      << "- #freenode and using the '/who freenode/staff/*' command. You may message\n"
      << "- any of us at any time. Please note that freenode predominantly provides\n"
      << "- assistance via private message, and while we have a network channel the\n"
      << "- primary venue for support requests is via private message to a member\n"
      << "- of the volunteer staff team.\n"
      << "-\n"
      << "- From time to time, volunteer staff may send server-wide notices relating to\n"
      << "- the project, or the communities that we host. The majority of such notices\n"
      << "- will be sent as wallops, and you can '/mode <yournick> +w' to ensure that you\n"
      << "- do not miss them. Important messages relating to the freenode project, including\n"
      << "- notices of upcoming maintenance and other scheduled downtime will be issued as\n"
      << "- global notices.\n"
      << "-\n"
      << "- Representing an on-topic project? Don't forget to register, more information\n"
      << "- can be found on the https://freenode.net website under \"Group Registration\".\n"
      << "-\n"
      << "- Thank you also to our server sponsors for the sustained support in keeping the\n"
      << "- network going for close to two decades.\n"
      << "-\n"
      << "- Thank you to all of our attendees, sponsors, speakers, exhibitors, helpers,\n"
      << "- and everyone else who made this year's freenode #live conference amazing.\n"
      << "- https://freenode.net/news/live-2018\n"
      << "-\n"
      << "- Thank you for using freenode!\n";

    EXPECT_EQ(motd.str(), sh.motd);
  }

  TEST_F(IrcServerTest, should_respond_to_ping_with_pong) {
    create_server();
    ss.str("");
    
    server->handle_message("PING :wolfe.freenode.net");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("PONG wolfe.freenode.net\r", line);
  }

  TEST_F(IrcServerTest, quit_without_message) {
    create_server();
    ss.str("");

    server->quit("");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("QUIT\r", line);

    EXPECT_EQ(true, sh.is_shutdown);
  }

  TEST_F(IrcServerTest, should_quit_with_message_by_sending_message_to_server) {
    create_server();
    ss.str("");

    server->quit("Goodbye, world!");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("QUIT :Goodbye, world!\r", line);

    EXPECT_EQ(true, sh.is_shutdown);
  }

  TEST_F(IrcServerTest, should_join_channel_by_sending_message_to_channel_and_handling_responses) {
    create_server();
    ss.str("");

    server->join("##c++");

    server->handle_message(":nick!jdoe@foo.org JOIN ##c++");
    server->handle_message(":wolfe.freenode.net 332 nick ##c++ :is a topical channel for discussing standard C++ specifications and code.");

    std::string line;
    getline(ss, line);
    EXPECT_EQ("JOIN ##c++\r", line);

    EXPECT_EQ(1, sh.channels.size());
    if (sh.channels.size() == 1) {
      EXPECT_EQ("##c++", sh.channels[0]->name);
      EXPECT_EQ("is a topical channel for discussing standard C++ specifications and code.", sh.channels[0]->topic);
    }
  }

  TEST_F(IrcServerTest, should_handle_quit_received_by_server_by_removing_from_repo_and_broadcasting_to_all_channels) {
    create_server();
    ss.str("");

    server->join("##c++");

    server->handle_message(":nick!jdoe@foo.org JOIN ##c++");
    server->handle_message(":wolfe.freenode.net 332 nick ##c++ :is a topical channel for discussing standard C++ specifications and code.");
    server->handle_message(":wolfe.freenode.net 353 nick * ##c++ :nick nick2");
    server->handle_message(":wolfe.freenode.net 366 nick ##c++ :End of /NAMES list.");

    auto user = entity_repo.find_user("nick2");
    EXPECT_EQ(true, (bool) user);
    
    server->handle_message(":nick2!foo@irc.org.bar QUIT :Goodbye world");

    user = entity_repo.find_user("nick2");
    EXPECT_EQ(false, (bool) user);

    auto server_quit_user = sh.quit_user;
    EXPECT_EQ(true, (bool) server_quit_user);
    if (server_quit_user) {
      EXPECT_EQ("nick2", *server_quit_user);
    }
    auto server_quit_message = sh.quit_message;
    EXPECT_EQ(true, (bool) server_quit_message);
    if (server_quit_message) {
      EXPECT_EQ("Goodbye world", *server_quit_message);
    }
    
    auto channel_quit_user = sh.channels[0]->quit_user;
    EXPECT_EQ(true, (bool) channel_quit_user);
    if (channel_quit_user) {
      EXPECT_EQ("nick2", *channel_quit_user);
    }
    auto channel_quit_message = sh.channels[0]->quit_message;
    EXPECT_EQ(true, (bool) channel_quit_message);
    if (channel_quit_message) {
      EXPECT_EQ("Goodbye world", *channel_quit_message);
    }
  }

  TEST_F(IrcServerTest, should_send_part_message_on_disconnecting_from_channel_and_send_disconnect_event) {
    create_server();

    server->join("##c++");
    server->handle_message(":nick!jdoe@foo.org JOIN ##c++");
    ss.str("");
    
    auto channel = entity_repo.find_channel(IrcMessage("JOIN", { "##c++" }));
    EXPECT_EQ(true, (bool) channel);
    if (channel) {
      channel->disconnect("Goodbye, channel!");

      std::string line;
      getline(ss, line);
      EXPECT_EQ("PART ##c++ :Goodbye, channel!\r", line);

      channel = entity_repo.find_channel(IrcMessage("JOIN", { "##c++" }));
      EXPECT_EQ(false, (bool) channel);
    }
  }

  TEST_F(IrcServerTest, should_handle_nick_change_message) {
    create_server();

    server->join("##c++");
    server->join("#haskell");
    server->handle_message(":nick!jdoe@foo.org JOIN ##c++");
    server->handle_message(":nick!jdoe@foo.org JOIN #haskell");
    server->handle_message(":nick2!jdoe@foo.org JOIN ##c++");
    EXPECT_EQ(true, (bool) entity_repo.find_user("nick2"));

    server->handle_message(":nick2!jdoe@foo.org NICK nick3");

    EXPECT_EQ(false, (bool) entity_repo.find_user("nick2"));
    auto user = entity_repo.find_user("nick3");
    EXPECT_EQ(true, (bool) user);
    if (user) {
      EXPECT_EQ("nick3", user->nickname());
    }

    EXPECT_EQ(true, (bool) sh.nick_from);
    if (sh.nick_from) {
      EXPECT_EQ("nick2", *sh.nick_from);
    }
    EXPECT_EQ(true, (bool) sh.nick_to);
    if (sh.nick_to) {
      EXPECT_EQ("nick3", *sh.nick_to);
    }

    auto ch = sh.channels[0];
    EXPECT_EQ(true, (bool) ch->nick_from);
    if (ch->nick_from) {
      EXPECT_EQ("nick2", *(ch->nick_from));
    }
    EXPECT_EQ(true, (bool) ch->nick_to);
    if (ch->nick_to) {
      EXPECT_EQ("nick3", *(ch->nick_to));
    }

    ch = sh.channels[1];
    EXPECT_EQ(false, (bool) ch->nick_from);
    EXPECT_EQ(false, (bool) ch->nick_to);
  }

  TEST_F(IrcServerTest, should_change_nick_by_sending_message_to_channel_and_handling_responses) {
    create_server();

    server->join("##c++");
    server->handle_message(":wolfe.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":nick!jdoe@foo.org JOIN ##c++");
    server->handle_message(":wolfe.freenode.net 332 nick ##c++ :is a topical channel for discussing standard C++ specifications and code.");
    server->handle_message(":wolfe.freenode.net 353 nick * ##c++ :nick");
    server->handle_message(":wolfe.freenode.net 366 nick ##c++ :End of /NAMES list.");
    ss.str("");

    auto user = entity_repo.find_user("nick");
    EXPECT_EQ(true, (bool) user);
    
    server->nick("nick2");
    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick2\r", line);

    server->handle_message(":nick!jdoe@foo.org NICK nick2");

    user = entity_repo.find_user("nick");
    EXPECT_EQ(false, (bool) user);

    user = entity_repo.find_user("nick2");
    EXPECT_EQ(true, (bool) user);

    EXPECT_EQ(true, (bool) sh.nick_from);
    if (sh.nick_from) {
      EXPECT_EQ("nick", *sh.nick_from);
    }
    EXPECT_EQ(true, (bool) sh.nick_to);
    if (sh.nick_to) {
      EXPECT_EQ("nick2", *sh.nick_to);
    }

    auto ch = sh.channels[0];
    EXPECT_EQ(true, (bool) ch->nick_from);
    if (ch->nick_from) {
      EXPECT_EQ("nick", *(ch->nick_from));
    }
    EXPECT_EQ(true, (bool) ch->nick_to);
    if (ch->nick_to) {
      EXPECT_EQ("nick2", *(ch->nick_to));
    }
  }

  TEST_F(IrcServerTest, should_handle_nick_response_in_trailing_parameter) {
    create_server();

    server->join("##c++");
    server->handle_message(":wolfe.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":nick!jdoe@foo.org JOIN ##c++");
    server->handle_message(":wolfe.freenode.net 332 nick ##c++ :is a topical channel for discussing standard C++ specifications and code.");
    server->handle_message(":wolfe.freenode.net 353 nick * ##c++ :nick");
    server->handle_message(":wolfe.freenode.net 366 nick ##c++ :End of /NAMES list.");
    ss.str("");

    auto user = entity_repo.find_user("nick");
    EXPECT_EQ(true, (bool) user);
    
    server->nick("nick2");
    std::string line;
    getline(ss, line);
    EXPECT_EQ("NICK nick2\r", line);

    server->handle_message(":nick!jdoe@foo.org NICK :nick2");

    user = entity_repo.find_user("nick");
    EXPECT_EQ(false, (bool) user);

    user = entity_repo.find_user("nick2");
    EXPECT_EQ(true, (bool) user);

    EXPECT_EQ(true, (bool) sh.nick_from);
    if (sh.nick_from) {
      EXPECT_EQ("nick", *sh.nick_from);
    }
    EXPECT_EQ(true, (bool) sh.nick_to);
    if (sh.nick_to) {
      EXPECT_EQ("nick2", *sh.nick_to);
    }

    auto ch = sh.channels[0];
    EXPECT_EQ(true, (bool) ch->nick_from);
    if (ch->nick_from) {
      EXPECT_EQ("nick", *(ch->nick_from));
    }
    EXPECT_EQ(true, (bool) ch->nick_to);
    if (ch->nick_to) {
      EXPECT_EQ("nick2", *(ch->nick_to));
    }
  }

  TEST_F(IrcServerTest, should_create_private_chat_event_handler_when_private_message_message_recieved) {
    create_server();

    server->handle_message(":wolfe.freenode.net 001 shorugoru :Welcome to the freenode Internet Relay Chat Network nick");
    server->handle_message(":freenode-connect!frigg@freenode/utility-bot/frigg PRIVMSG shorugoru :Hello");
    server->handle_message(":freenode-connect!frigg@freenode/utility-bot/frigg PRIVMSG shorugoru :World");
    ss.str("");

    auto user = entity_repo.find_user("freenode-connect");
    EXPECT_EQ(true, (bool) user);

    EXPECT_EQ(1, sh.private_chats.size());
    if (sh.private_chats.size() == 1) {
      EXPECT_EQ(2, sh.private_chats[0]->private_msgs.size());
      if (sh.private_chats[0]->private_msgs.size() == 2) {
        EXPECT_EQ("Hello", sh.private_chats[0]->private_msgs[0]);
        EXPECT_EQ("World", sh.private_chats[0]->private_msgs[1]);
      }
    }
  }

}
