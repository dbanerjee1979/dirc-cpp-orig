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
    IrcMessage msg(msg_str = "PING\r\n");

    EXPECT_EQ("PING", msg.command);
  }

  TEST_F(IrcMessageTest, test_basic_message_multiple_basic_arguments) {
    IrcMessage msg(msg_str = "MODE WiZ -w\r\n");

    EXPECT_EQ("MODE", msg.command);
    EXPECT_EQ(2, msg.params.size());
    if (msg.params.size() == 2) {
      EXPECT_EQ("WiZ", msg.params[0]);
      EXPECT_EQ("-w", msg.params[1]);
    }
  }

  TEST_F(IrcMessageTest, test_basic_message_multiple_basic_arguments_and_trailing_argument) {
    IrcMessage msg(msg_str = "USER guest 0 * :Ronnie Reagan\r\n");

    EXPECT_EQ("USER", msg.command);
    EXPECT_EQ(3, msg.params.size());
    if (msg.params.size() == 3) {
      EXPECT_EQ("guest", msg.params[0]);
      EXPECT_EQ("0", msg.params[1]);
      EXPECT_EQ("*", msg.params[2]);
    }
    EXPECT_EQ("Ronnie Reagan", msg.trailing);
  }

  TEST_F(IrcMessageTest, test_message_with_server_name) {
    IrcMessage msg(msg_str = ":tolsun.oulu.fi PING\r\n");

    EXPECT_EQ("tolsun.oulu.fi", msg.servername);
    EXPECT_EQ("PING", msg.command);
  }

  TEST_F(IrcMessageTest, test_message_with_nick) {
    IrcMessage msg(msg_str = ":Angel INVITE Wiz #Dust\r\n");

    EXPECT_EQ("Angel", msg.nick);
    EXPECT_EQ("INVITE", msg.command);
    EXPECT_EQ(2, msg.params.size());
    if (msg.params.size() == 2) {
      EXPECT_EQ("Wiz", msg.params[0]);
      EXPECT_EQ("#Dust", msg.params[1]);
    }
  }
}
