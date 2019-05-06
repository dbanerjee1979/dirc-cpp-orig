#include <gtest/gtest.h>
#include <core/channel_event_handler.h>
#include <core/irc_entity_repository.h>
#include <core/irc_message_commands.h>

namespace core {

  class IrcEntityRepositoryTest : public testing::Test {
  protected:
    IrcEntityRepositoryTest() :
      event_handler (new ChannelEventHandler()) {
      std::string channel = "##c++";
      entity_repo.create_channel(channel, event_handler);
    }
    
    IrcEntityRepository entity_repo;
    ChannelEventHandler *event_handler;
  };

  TEST_F(IrcEntityRepositoryTest, test_channel_lookup_by_topic_message) {
    IrcMessage msg = IrcMessage(RPL_TOPIC, { "nick", "##c++" }, "Test");
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(true, found);
    
    if (found) {
      ASSERT_EQ("##c++", ch->name());
    }
  }

  TEST_F(IrcEntityRepositoryTest, test_return_none_for_channel_not_found) {
    IrcMessage msg = IrcMessage(RPL_TOPIC, { "nick", "#haskell" }, "Test");
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(false, found);
  }

  
  TEST_F(IrcEntityRepositoryTest, test_return_none_for_missing_channel_in_message) {
    IrcMessage msg = IrcMessage(RPL_TOPIC, { "nick" }, "Test");
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(false, found);
  }
  
}
