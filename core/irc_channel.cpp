#include "irc_channel.h"
#include "irc_message_commands.h"
#include "irc_message_parser.h"
#include "user_event_handler.h"

namespace core {

  IrcChannel::IrcChannel(const std::string &name,
                         std::ostream &out,
                         IrcUserRepository &user_repo) :
    m_name(name),
    m_out(out),
    m_user_repo(user_repo) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_TOPIC] = std::bind(&IrcChannel::handle_topic, this, _1);
    m_msg_handlers[RPL_NOTOPIC] = std::bind(&IrcChannel::handle_no_topic, this, _1);
    m_msg_handlers[RPL_NAMREPLY] = std::bind(&IrcChannel::handle_name_reply, this, _1);
    m_msg_handlers[RPL_ENDOFNAMES] = std::bind(&IrcChannel::handle_name_reply_end, this, _1);
    m_msg_handlers["JOIN"] = std::bind(&IrcChannel::handle_join, this, _1);
    m_msg_handlers["PART"] = std::bind(&IrcChannel::handle_part, this, _1);

  }

  std::string &IrcChannel::name() {
    return m_name;
  }

  void IrcChannel::disconnect(const std::string &msg) {
    m_out << IrcMessage("PART", { m_name }, msg).str() << std::endl;
    send_event([&] (ChannelEventHandler &h) { h.disconnected(); });
  }

  void IrcChannel::handle_topic(const IrcMessage &msg) {
    send_event([&] (ChannelEventHandler &h) { h.topic_changed(msg.trailing()); });
  }

  void IrcChannel::handle_no_topic(const IrcMessage &msg) {
    send_event([&] (ChannelEventHandler &h) { h.topic_changed(""); });
  }

  void IrcChannel::handle_name_reply(const IrcMessage &msg) {
    std::vector<std::string> nicks;
    boost::split(nicks, msg.trailing(), boost::is_any_of(" "));
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
    struct ChannelUserHandler : public UserEventHandler {
      IrcChannel &m_ch;
      IrcChannelUser &m_user;
      ChannelUserHandler(IrcChannel &ch, IrcChannelUser &user) : m_ch(ch), m_user(user) {
      }
      void nick_changed(const std::string &nick_from, const std::string &nick_to) {
        m_ch.send_event([&] (ChannelEventHandler &h) { h.nick_changed(nick_from, nick_to); });
      }
      void quit(const std::string &msg) {
        m_ch.send_event([&] (ChannelEventHandler &h) { h.user_quit(m_user, msg); });
      }
    };
    if (!m_user_repo.find_user(nick)) {
      m_user_repo.create_user(nick, username, "");
    }
    m_users.push_back(std::unique_ptr<IrcChannelUser>(new IrcChannelUser(*m_user_repo.find_user(nick), chan_mode)));
    IrcChannelUser &user = *m_users.back();
    user.user().add_user_event_handler(std::shared_ptr<ChannelUserHandler>(new ChannelUserHandler(*this, user)));
    return user;
  }
  
  void IrcChannel::handle_name_reply_end(const IrcMessage &msg) {
    send_event([&] (ChannelEventHandler &h) { h.channel_users(m_users.begin(), m_users.end()); });
  }

  void IrcChannel::handle_join(const IrcMessage &msg) {
    auto user = add_user(msg.nick(), msg.user(), "");
    send_event([&] (ChannelEventHandler &h) { h.user_joined(user); });
  }

  void IrcChannel::handle_part(const IrcMessage &msg) {
    for (auto it = m_users.begin(); it != m_users.end(); it++) {
      if ((*it)->user().nickname() == msg.nick()) {
        send_event([&] (ChannelEventHandler &h) { h.user_departed(**it, msg.trailing()); });
        m_users.erase(it);
        m_user_repo.remove_user(msg.nick());
        break;
      }
    }
  }

}
