#include "irc_channel.h"
#include "irc_message_commands.h"
#include "irc_message_parser.h"

namespace core {

  IrcChannel::IrcChannel(std::string &name,
                         ChannelEventHandler *event_handler,
                         IrcUserRepository &user_repo) :
    m_name(name), 
    m_event_handler(event_handler),
    m_user_repo(user_repo) {

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
    m_event_handler->topic_changed("");
  }

  void IrcChannel::handle_name_reply(IrcMessage &msg) {
    std::vector<std::string> nicks;
    boost::split(nicks, msg.trailing, boost::is_any_of(" "));
    for (auto it = nicks.begin(); it != nicks.end(); it++) {
      std::string chan_mode;
      std::string nick;
      IrcMessageParser p(*it);
      if (p.is_nickname()) {
        p.token(nick);
      }
      else {
        p.skip();
        p.token(chan_mode);
        if (p.is_nickname()) {
          p.token(nick);
        }
      }

      if (!nick.empty()) {
        if (!m_user_repo.find_user(nick)) {
          m_user_repo.create_user(nick);
        }
        m_users.push_back(IrcChannelUser(*m_user_repo.find_user(nick), chan_mode));
      }
    }
  }
  
  void IrcChannel::handle_name_reply_end(IrcMessage &msg) {
    m_event_handler->channel_users(m_users);
    m_users.clear();
  }
  
}
