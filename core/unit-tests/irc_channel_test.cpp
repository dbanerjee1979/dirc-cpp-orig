#include <gtest/gtest.h>
#include <core/channel_event_handler.h>
#include <core/irc_entity_repository.h>
#include <core/irc_message_commands.h>

namespace core {

  class StubChannelEventHandlerCT : public ChannelEventHandler {
  public:
    StubChannelEventHandlerCT() {
    }

    virtual ~StubChannelEventHandlerCT() {
    }

    void topic_changed(std::string& msg) {
      topic = msg;
    }

    std::string topic;
  };

  class IrcChannelTest : public testing::Test {
  protected:
    IrcChannelTest() :
      event_handler (new StubChannelEventHandlerCT()),
      name ("##c++"),
      channel (name, event_handler) {
    }

    StubChannelEventHandlerCT *event_handler;
    std::string name;
    IrcChannel channel;
  };

  TEST_F(IrcChannelTest, test_return_none_for_channel_not_found) {
    IrcMessage msg = IrcMessage(RPL_TOPIC, { "nick", "#haskell" }, "Test");

    channel.handle_message(msg);

    ASSERT_EQ("Test", event_handler->topic);
  }
  
}
