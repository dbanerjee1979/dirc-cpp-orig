#include "irc_channel.h"
#include "irc_message_commands.h"
#include "irc_message_parser.h"

namespace core {

  IrcChannel::IrcChannel(const std::string &name,
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
    m_msg_handlers["QUIT"] = std::bind(&IrcChannel::handle_quit, this, _1);
    m_msg_handlers["JOIN"] = std::bind(&IrcChannel::handle_join, this, _1);
  }

  std::string &IrcChannel::name() {
    return m_name;
  }

  void IrcChannel::handle_message(const IrcMessage &msg) {
    auto it = m_msg_handlers.find(msg.command);
    if (it != m_msg_handlers.end()) {
      it->second(msg);
    }
  }

  void IrcChannel::handle_topic(const IrcMessage &msg) {
    m_event_handler->topic_changed(msg.trailing);
  }

  void IrcChannel::handle_no_topic(const IrcMessage &msg) {
    m_event_handler->topic_changed("");
  }

  void IrcChannel::handle_name_reply(const IrcMessage &msg) {
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
        add_user(nick, "", chan_mode);
      }
    }
  }

  IrcChannelUser &IrcChannel::add_user(const std::string &nick, const std::string &username, const std::string &chan_mode) {
    if (!m_user_repo.find_user(nick)) {
      m_user_repo.create_user(nick, username);
    }
    auto user = new IrcChannelUser(*m_user_repo.find_user(nick), chan_mode);
    m_users.push_back(std::unique_ptr<IrcChannelUser>(user));
    return *user;
  }
  
  void IrcChannel::handle_name_reply_end(const IrcMessage &msg) {
    std::vector<IrcChannelUser *> users;
    for (auto it = m_users.begin(); it != m_users.end(); it++) {
      users.push_back(it->get());
    }
    m_event_handler->channel_users(users);
  }

  void IrcChannel::handle_quit(const IrcMessage &msg) {
    for (auto it = m_users.begin(); it != m_users.end(); it++) {
      if ((*it)->user().nickname() == msg.nick) {
        m_event_handler->user_quit(**it, msg.trailing);
        break;
      }
    }
  }

  void IrcChannel::handle_join(const IrcMessage &msg) {
    auto user = add_user(msg.nick, msg.user, "");
    m_event_handler->user_joined(user);
  }
  
}
