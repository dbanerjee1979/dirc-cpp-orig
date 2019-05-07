#include "irc_channel.h"

#include "irc_message_commands.h"

namespace core {

  IrcChannel::IrcChannel(std::string &name, ChannelEventHandler *event_handler) :
    m_name(name), 
    m_event_handler(event_handler) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_TOPIC] = std::bind(&IrcChannel::handle_topic, this, _1);
    m_msg_handlers[RPL_NOTOPIC] = std::bind(&IrcChannel::handle_no_topic, this, _1);
    m_msg_handlers[RPL_NAMREPLY] = std::bind(&IrcChannel::handle_name_reply, this, _1);
    m_msg_handlers[RPL_ENDOFNAMES] = std::bind(&IrcChannel::handle_name_reply_end, this, _1);
  }

  std::string &IrcChannel::name() {
    return m_name;
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

  void IrcChannel::handle_no_topic(IrcMessage &msg) {
    std::string empty_msg = "";
    m_event_handler->topic_changed(empty_msg);
  }

  void IrcChannel::handle_name_reply(IrcMessage &msg) {
    std::vector<std::string> nicks;
    boost::split(nicks, msg.trailing, boost::is_any_of(" "));
    for (auto it = nicks.begin(); it != nicks.end(); it++) {
      m_users.push_back(IrcUser(*it));
    }
  }
  
  void IrcChannel::handle_name_reply_end(IrcMessage &msg) {
    std::vector<IrcUser *> users;
    for (auto it = m_users.begin(); it != m_users.end(); it++) {
      users.push_back(&(*it));
    }
    m_event_handler->channel_users(users);
    m_users.clear();
  }
  
}
