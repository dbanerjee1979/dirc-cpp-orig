#include <gtest/gtest.h>
#include <core/irc_message.h>

namespace core {
  class IrcMessageTest : public testing::Test {
  protected:
    IrcMessageTest() {
    }

    ~IrcMessageTest() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    std::string msg_str;
  };

  TEST_F(IrcMessageTest, test_command) {
    IrcMessage msg("PING\r\n");

    EXPECT_EQ("PING", msg.command());
  }

  TEST_F(IrcMessageTest, test_basic_message_multiple_basic_arguments) {
    IrcMessage msg("MODE WiZ -w\r\n");

    EXPECT_EQ("MODE", msg.command());
    EXPECT_EQ(2, msg.param_count());
    EXPECT_EQ("WiZ", msg.param(0));
    EXPECT_EQ("-w", msg.param(1));
  }

  TEST_F(IrcMessageTest, test_basic_message_multiple_basic_arguments_and_trailing_argument) {
    IrcMessage msg("USER guest 0 * :Ronnie Reagan\r\n");

    EXPECT_EQ("USER", msg.command());
    EXPECT_EQ(4, msg.param_count());
    EXPECT_EQ("guest", msg.param(0));
    EXPECT_EQ("0", msg.param(1));
    EXPECT_EQ("*", msg.param(2));
    EXPECT_EQ("Ronnie Reagan", msg.param(3));
    EXPECT_EQ("Ronnie Reagan", msg.trailing());
  }

  TEST_F(IrcMessageTest, test_message_with_server_name) {
    IrcMessage msg(":tolsun.oulu.fi PING\r\n");

    EXPECT_EQ("tolsun.oulu.fi", msg.servername());
    EXPECT_EQ("PING", msg.command());
  }

  TEST_F(IrcMessageTest, test_server_name_cannot_be_just_a_dot) {
    IrcMessage msg(":. PING\r\n");

    EXPECT_EQ("", msg.servername());
    EXPECT_EQ("", msg.command());
  }

  TEST_F(IrcMessageTest, test_server_name_cannot_start_with_a_dot) {
    IrcMessage msg(":.com PING\r\n");

    EXPECT_EQ("", msg.servername());
    EXPECT_EQ("", msg.command());
  }

  TEST_F(IrcMessageTest, test_message_with_nick) {
    IrcMessage msg(":Angel INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick());
    EXPECT_EQ("INVITE", msg.command());
    EXPECT_EQ(2, msg.param_count());
    EXPECT_EQ("Wiz", msg.param(0));
    EXPECT_EQ("#Dust", msg.param(1));
  }

  TEST_F(IrcMessageTest, test_message_with_nick_with_special_chars) {
    IrcMessage msg(":_A`{ngel}|[1-2-3]\\^!wings@irc.org INVITE Wiz #Dust\r\n");

    EXPECT_EQ("_A`{ngel}|[1-2-3]\\^", msg.nick());
    EXPECT_EQ("INVITE", msg.command());
    EXPECT_EQ(2, msg.param_count());
    EXPECT_EQ("Wiz", msg.param(0));
    EXPECT_EQ("#Dust", msg.param(1));
  }

  TEST_F(IrcMessageTest, test_message_with_nick_and_host) {
    IrcMessage msg(":Angel@irc.org INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick());
    EXPECT_EQ("irc.org", msg.host());
    EXPECT_EQ("INVITE", msg.command());
    EXPECT_EQ(2, msg.param_count());
    EXPECT_EQ("Wiz", msg.param(0));
    EXPECT_EQ("#Dust", msg.param(1));
  }

  TEST_F(IrcMessageTest, test_message_cannot_have_host_symbol_and_no_host) {
    IrcMessage msg(":Angel@ INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick());
    EXPECT_EQ("", msg.host());
    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
  }

  TEST_F(IrcMessageTest, test_message_cannot_have_host_without_nick) {
    IrcMessage msg(":@irc.org INVITE Wiz #Dust\r\n");

    EXPECT_EQ("", msg.nick());
    EXPECT_EQ("", msg.host());
    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
  }
  
  TEST_F(IrcMessageTest, test_message_with_nick_and_user_and_host) {
    IrcMessage msg(":Angel!wings@irc.org INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick());
    EXPECT_EQ("wings", msg.user());
    EXPECT_EQ("irc.org", msg.host());
    EXPECT_EQ("INVITE", msg.command());
    EXPECT_EQ(2, msg.param_count());
    EXPECT_EQ("Wiz", msg.param(0));
    EXPECT_EQ("#Dust", msg.param(1));
  }

  TEST_F(IrcMessageTest, test_message_cannot_have_user_symbol_with_no_user) {
    IrcMessage msg(":Angel!@irc.org INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick());
    EXPECT_EQ("", msg.user());
    EXPECT_EQ("", msg.host());
    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
  }

  TEST_F(IrcMessageTest, test_message_cannot_have_user_without_host) {
    IrcMessage msg(":Angel!wings INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick());
    EXPECT_EQ("", msg.user());
    EXPECT_EQ("", msg.host());
    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
    EXPECT_EQ("", msg.trailing());
  }
    
  TEST_F(IrcMessageTest, test_trailing_param_only) {
    IrcMessage msg("QUIT :Goodbye, cruel world!\r\n");

    EXPECT_EQ("QUIT", msg.command());
    EXPECT_EQ(1, msg.param_count());
    EXPECT_EQ("Goodbye, cruel world!", msg.trailing());
  }

  TEST_F(IrcMessageTest, test_trailing_param_only_with_no_length) {
    IrcMessage msg("QUIT :\r\n");

    EXPECT_EQ("QUIT", msg.command());
    EXPECT_EQ(0, msg.param_count());
    EXPECT_EQ("", msg.trailing());
  }

  TEST_F(IrcMessageTest, test_numeric_message) {
    IrcMessage msg(":rajaniemi.freenode.net 001 nick :Welcome to the freenode Internet Relay Chat Network nick\r\n");

    EXPECT_EQ("rajaniemi.freenode.net", msg.servername());
    EXPECT_EQ("001", msg.command());
    EXPECT_EQ(2, msg.param_count());
    EXPECT_EQ("nick", msg.param(0));
    EXPECT_EQ("Welcome to the freenode Internet Relay Chat Network nick", msg.trailing());
  }

  
  TEST_F(IrcMessageTest, test_numeric_message_with_too_many_digits) {
    IrcMessage msg(":rajaniemi.freenode.net 0013 nick :Welcome to the freenode Internet Relay Chat Network nick\r\n");

    EXPECT_EQ("rajaniemi.freenode.net", msg.servername());
    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
    EXPECT_EQ("", msg.trailing());
  }

  TEST_F(IrcMessageTest, test_numeric_message_with_too_few_digits) {
    IrcMessage msg(":rajaniemi.freenode.net 13 nick :Welcome to the freenode Internet Relay Chat Network nick\r\n");

    EXPECT_EQ("rajaniemi.freenode.net", msg.servername());
    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
    EXPECT_EQ("", msg.trailing());
  }

  TEST_F(IrcMessageTest, command_can_only_have_alpha_and_digit) {
    IrcMessage msg("!QUIT :Goodbye, cruel world!\r\n");

    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
    EXPECT_EQ("", msg.trailing());
  }

  TEST_F(IrcMessageTest, should_not_fail_with_unparseable_message) {
    IrcMessage msg("~~~~~");

    EXPECT_EQ("", msg.command());
    EXPECT_EQ(0, msg.param_count());
    EXPECT_EQ("", msg.trailing());
  }
}
