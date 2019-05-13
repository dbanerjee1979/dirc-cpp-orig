#include "irc_user.h"

namespace core {

  IrcUser::IrcUser(const std::string &nickname,
                   const std::string &username,
                   const std::string &realname) :
    m_nickname(nickname),
    m_username(username),
    m_realname(realname) {

    using std::placeholders::_1;
    m_msg_handlers["NICK"] = std::bind(&IrcUser::handle_nick, this, _1);
    m_msg_handlers["QUIT"] = std::bind(&IrcUser::handle_quit, this, _1);
  }

  const std::string &IrcUser::nickname() {
    return m_nickname;
  }

  void IrcUser::nickname(const std::string &nickname) {
    m_nickname = nickname;
  }

  const std::string &IrcUser::username() {
    return m_username;
  }
  
  const std::string &IrcUser::realname() {
    return m_realname;
  }

  void IrcUser::handle_nick(const IrcMessage &msg) {
    const std::string &nick_from = msg.nick();
    const std::string &nick_to = msg.param(0);
    if (!nick_to.empty()) {
      m_nickname = nick_to;
      send_event([&] (UserEventHandler &h) { h.nick_changed(nick_from, nick_to); });
    }
  }

  void IrcUser::handle_quit(const IrcMessage &msg) {
    send_event([&] (UserEventHandler &h) { h.quit(msg.trailing()); });
  }
}
