#include "irc_channel.h"

#include "irc_message_commands.h"

namespace core {

  IrcChannel::IrcChannel(ChannelEventHandler *event_handler) :
    m_event_handler(event_handler) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_TOPIC] = std::bind(&IrcChannel::handle_topic, this, _1);
  }

  void IrcChannel::handle_message(IrcMessage &msg) {
    auto it = m_msg_handlers.find(msg.command);
    if (it != m_msg_handlers.end()) {
      it->second(msg);
    }
  }

  void IrcChannel::handle_topic(IrcMessage &msg) {
    m_event_handler->topic_changed(msg.trailing);
  }
}
