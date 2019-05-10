#include <gtest/gtest.h>
#include <core/channel_event_handler.h>
#include <core/irc_entity_repository.h>
#include <core/irc_message_commands.h>
#include <core/irc_channel_user.h>

namespace core {

  class StubChannelEventHandlerCT : public ChannelEventHandler {
  public:
    StubChannelEventHandlerCT() {
    }

    virtual ~StubChannelEventHandlerCT() {
    }

    void topic_changed(const std::string& _topic) {
      topic = _topic;
    }

    void channel_users(const std::vector<IrcChannelUser *> &_users) {
      users = _users;
    }

    void user_joined(IrcChannelUser &_user) {
      joined_user = _user;
    }

    std::string topic;
    std::vector<IrcChannelUser *> users;
    boost::optional<IrcChannelUser> joined_user;
  };

  class IrcChannelTest : public testing::Test {
  protected:
    IrcChannelTest() :
      event_handler (new StubChannelEventHandlerCT()),
      name ("##c++"),
      channel (name, event_handler, entity_repo) {
    }

    IrcEntityRepository entity_repo;
    StubChannelEventHandlerCT *event_handler;
    std::string name;
    IrcChannel channel;
  };

  TEST_F(IrcChannelTest, should_handle_channel_topic_change) {
    IrcMessage msg = IrcMessage(RPL_TOPIC, { "nick", "##c++" }, "New topic");

    channel.handle_message(msg);

    ASSERT_EQ("New topic", event_handler->topic);
  }

  TEST_F(IrcChannelTest, should_handle_channel_topic_change_to_no_topic) {
    event_handler->topic = "Dummy";
    IrcMessage msg = IrcMessage(RPL_NOTOPIC, { "nick", "##c++" }, "No topic");

    channel.handle_message(msg);

    ASSERT_EQ("", event_handler->topic);
  }

  TEST_F(IrcChannelTest, should_handle_names_list) {
    entity_repo.create_user("nick", "jdoe", "John Doe");
    
    IrcMessage msg1 = IrcMessage(RPL_NAMREPLY, { "nick", "*", "##c++" }, "nick nick2 nick3");
    IrcMessage msg2 = IrcMessage(RPL_NAMREPLY, { "nick", "*", "##c++" }, "nick4 nick5 nick6");
    IrcMessage msg3 = IrcMessage(RPL_ENDOFNAMES, { "nick", "##c++" }, "End of /NAMES list.");
    
    channel.handle_message(msg1);
    channel.handle_message(msg2);
    channel.handle_message(msg3);

    auto users = event_handler->users;
    ASSERT_EQ(6, users.size());
    if (users.size() == 6) {
      ASSERT_EQ("nick", users[0]->user().nickname());
      ASSERT_EQ("jdoe", users[0]->user().username());
      ASSERT_EQ("John Doe", users[0]->user().realname());
      ASSERT_EQ("nick2", users[1]->user().nickname());
      ASSERT_EQ("nick3", users[2]->user().nickname());
      ASSERT_EQ("nick4", users[3]->user().nickname());
      ASSERT_EQ("nick5", users[4]->user().nickname());
      ASSERT_EQ("nick6", users[5]->user().nickname());
    }

    std::vector<std::string> nicks = { "nick", "nick2", "nick3", "nick4", "nick5", "nick6" };
    for (auto it = nicks.begin(); it != nicks.end(); it++) {
      auto nick = entity_repo.find_user(*it);
      ASSERT_EQ(true, (bool) nick);
      if (nick) {
        ASSERT_EQ(*it, nick->nickname());
      }
    }
  }
  
  TEST_F(IrcChannelTest, should_handle_channel_prefixes_in_names_list) {
    entity_repo.create_user("nick", "jdoe", "John Doe");
    
    IrcMessage msg1 = IrcMessage(RPL_NAMREPLY, { "nick", "*", "##c++" }, "nick ~nick2 &nick3 nick4");
    IrcMessage msg2 = IrcMessage(RPL_NAMREPLY, { "nick", "*", "##c++" }, "@nick5 %nick6 +nick7 nick8");
    IrcMessage msg3 = IrcMessage(RPL_ENDOFNAMES, { "nick", "##c++" }, "End of /NAMES list.");
    
    channel.handle_message(msg1);
    channel.handle_message(msg2);
    channel.handle_message(msg3);

    auto users = event_handler->users;
    ASSERT_EQ(8, users.size());
    if (users.size() == 8) {
      ASSERT_EQ("nick", users[0]->user().nickname());
      ASSERT_EQ("jdoe", users[0]->user().username());
      ASSERT_EQ("John Doe", users[0]->user().realname());
      ASSERT_EQ(ChanModeNone, users[0]->channel_mode());
      ASSERT_EQ("nick2", users[1]->user().nickname());
      ASSERT_EQ(ChanModeFounder, users[1]->channel_mode());
      ASSERT_EQ("nick3", users[2]->user().nickname());
      ASSERT_EQ(ChanModeProtected, users[2]->channel_mode());
      ASSERT_EQ("nick4", users[3]->user().nickname());
      ASSERT_EQ(ChanModeNone, users[3]->channel_mode());
      ASSERT_EQ("nick5", users[4]->user().nickname());
      ASSERT_EQ(ChanModeOp, users[4]->channel_mode());
      ASSERT_EQ("nick6", users[5]->user().nickname());
      ASSERT_EQ(ChanModeHalfOp, users[5]->channel_mode());
      ASSERT_EQ("nick7", users[6]->user().nickname());
      ASSERT_EQ(ChanModeVoice, users[6]->channel_mode());
      ASSERT_EQ("nick8", users[7]->user().nickname());
      ASSERT_EQ(ChanModeNone, users[7]->channel_mode());
    }
  }

  TEST_F(IrcChannelTest, should_handle_new_user_joining_channel) {
    channel.handle_message(IrcMessage(":nick!jdoe@foo.org JOIN ##c++"));

    auto user = event_handler->joined_user;
    ASSERT_EQ(true, (bool) user);
    if (user) {
      ASSERT_EQ("nick", user->user().nickname());
      ASSERT_EQ("jdoe", user->user().username());
    }

    std::vector<std::string> nicks = { "nick" };
    for (auto it = nicks.begin(); it != nicks.end(); it++) {
      auto nick = entity_repo.find_user(*it);
      ASSERT_EQ(true, (bool) nick);
      if (nick) {
        ASSERT_EQ(*it, nick->nickname());
      }
    }    
  }
  
}
