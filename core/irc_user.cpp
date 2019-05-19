#include <boost/optional.hpp>
#include "irc_user.h"
#include "chat_event_handler.h"

namespace core {
  IrcUser::IrcUser(const std::string &nickname,
                   const std::string &username,
                   const std::string &realname,
                   const std::shared_ptr<ChatEventHandlerFactory> chat_factory) :
    m_nickname(nickname),
    m_username(username),
    m_realname(realname),
    m_is_client(!realname.empty()),
    m_chat_factory(chat_factory) {

    using std::placeholders::_1;
    m_msg_handlers["NICK"] = std::bind(&IrcUser::handle_nick, this, _1);
    m_msg_handlers["QUIT"] = std::bind(&IrcUser::handle_quit, this, _1);
    m_msg_handlers["PRIVMSG"] = std::bind(&IrcUser::handle_private_chat, this, _1);
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

  bool IrcUser::is_client() {
    return m_is_client;
  }

  void IrcUser::add_user_event_handler(std::shared_ptr<UserEventHandler> handler) {
    IrcEventHandlerMixin<UserEventHandler>::add_event_handler(handler);
  }

  void IrcUser::add_chat_event_handler(std::shared_ptr<ChatEventHandler> handler) {
    IrcEventHandlerMixin<ChatEventHandler>::add_event_handler(handler);
  }

  void IrcUser::handle_nick(const IrcMessage &msg) {
    const std::string &nick_from = msg.nick();
    const std::string &nick_to = msg.param(0);
    if (!nick_to.empty()) {
      m_nickname = nick_to;
      IrcEventHandlerMixin<UserEventHandler>::send_event([&] (UserEventHandler &h) { h.nick_changed(nick_from, nick_to); });
    }
  }

  void IrcUser::handle_quit(const IrcMessage &msg) {
    IrcEventHandlerMixin<UserEventHandler>::send_event([&] (UserEventHandler &h) { h.quit(msg.trailing()); });
  }

  void IrcUser::handle_private_chat(const IrcMessage &msg) {
    if (m_chat_factory && !m_chat) {
      m_chat = m_chat_factory->create_chat_event_handler();
    }
    if (m_chat) {
      m_chat->recieved_message(msg.trailing());
    }
  }
}
