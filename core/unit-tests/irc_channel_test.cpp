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

  TEST_F(IrcChannelTest, test_handle_topic_change_message) {
    IrcMessage msg = IrcMessage(RPL_TOPIC, { "nick", "##c++" }, "New topic");

    channel.handle_message(msg);

    ASSERT_EQ("New topic", event_handler->topic);
  }

  TEST_F(IrcChannelTest, test_handle_no_topic_change_message) {
    event_handler->topic = "Dummy";
    IrcMessage msg = IrcMessage(RPL_NOTOPIC, { "nick", "##c++" }, "No topic");

    channel.handle_message(msg);

    ASSERT_EQ("", event_handler->topic);
  }

  TEST_F(IrcChannelTest, test_handle_names_list) {
    event_handler->topic = "Dummy";

    /*
    :barjavel.freenode.net 353 shorugoru * ##c++ :sssilver obiwahn hagabaka m712 yinflying coincoin169 riotz richardwhiuk drozdziak1 nilspin jgowdy hirish tomaw mgrech_ choco anderson pocketprotector nukedclx dbarrett M-ou-se iamthad_ heinrich5991 dau glenfe Jim_D` Peixinho JWatkins H1N1theI S007 Hansformer ploks Kronuz paperManu Nothing4You_ graphitemaster Deaod ponbiki Nebraskka ColdKeyboard stowelly sta|ker abra0 yan kloeri cbdev bodie_ ericP karstensrage sov9 fury__ Kitlith georgemp tabakhase Buzzer
:barjavel.freenode.net 366 shorugoru ##c++ :End of /NAMES list.
    */

    
    IrcMessage msg = IrcMessage(RPL_NOTOPIC, { "nick", "##c++" }, "No topic");

    channel.handle_message(msg);

    ASSERT_EQ("", event_handler->topic);
  }
  
}
