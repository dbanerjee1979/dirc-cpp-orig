#include <gtest/gtest.h>
#include <core/channel_event_handler.h>
#include <core/irc_entity_repository.h>
#include <core/irc_message_commands.h>

namespace core {

  class IrcEntityRepositoryTest : public testing::Test {
  protected:
    IrcEntityRepositoryTest() :
      event_handler(new ChannelEventHandler()) {
      std::string channel = "##c++";
      entity_repo.create_channel(channel, out, std::shared_ptr<ChannelEventHandler>(event_handler));
      entity_repo.create_user("nick");
    }

    std::stringstream out;
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

  TEST_F(IrcEntityRepositoryTest, test_channel_lookup_by_name_reply_message) {
    IrcMessage msg = IrcMessage(RPL_NAMREPLY, { "nick", "*", "##c++" }, "nick1 nick2 nick3");
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(true, found);
    
    if (found) {
      ASSERT_EQ("##c++", ch->name());
    }
  }
  
  TEST_F(IrcEntityRepositoryTest, test_channel_lookup_by_name_reply_end_message) {
    IrcMessage msg = IrcMessage(RPL_ENDOFNAMES, { "nick", "##c++" }, "End of /NAMES list.");
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(true, found);
    
    if (found) {
      ASSERT_EQ("##c++", ch->name());
    }
  }

  TEST_F(IrcEntityRepositoryTest, test_channel_lookup_by_no_topic_message) {
    IrcMessage msg = IrcMessage(RPL_NOTOPIC, { "nick", "##c++" }, "No topic set");
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(true, found);
    
    if (found) {
      ASSERT_EQ("##c++", ch->name());
    }
  }

  TEST_F(IrcEntityRepositoryTest, test_channel_lookup_by_part_message) {
    IrcMessage msg = IrcMessage("PART", { "##c++" });
    auto ch = entity_repo.find_channel(msg);

    bool found = (bool) ch;
    ASSERT_EQ(true, found);
    
    if (found) {
      ASSERT_EQ("##c++", ch->name());
    }
  }

  TEST_F(IrcEntityRepositoryTest, test_user_lookup_by_part_message) {
    IrcMessage msg = IrcMessage(":nick!jdoe@foo.org NICK nick2");
    auto user = entity_repo.find_user(msg);

    bool found = (bool) user;
    ASSERT_EQ(true, found);
    
    if (found) {
      ASSERT_EQ("nick", user->nickname());
    }
  }
}
