#include "irc_server.h"
#include "irc_server_commands.h"
#include "irc_message.h"

namespace core {
  IrcServer::IrcServer(config::Network &network,
		       std::ostream &out,
		       ServerEventHandler &server_event_handler) :
    m_out(out),
    m_server_event_handler(server_event_handler),
    m_nicks(network.user_info.nicks),
    m_nick_id(0) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_WELCOME] = std::bind(&IrcServer::handle_connection_registration, *this, _1);
    m_msg_handlers[ERR_NICKNAMEINUSE] = std::bind(&IrcServer::handle_nick_error, *this, _1);
    m_msg_handlers[ERR_NICKCOLLISION] = std::bind(&IrcServer::handle_nick_error, *this, _1);

    if (!network.user_info.password.empty()) {
      m_out << IrcMessage("PASSWORD", { network.user_info.password }).str() << std::flush;
    }
    m_out << IrcMessage("NICK", { m_nicks[m_nick_id] }).str() << std::flush;
    m_out << IrcMessage("USER", { network.user_info.username, "8", "*" }, network.user_info.realname).str() << std::flush;
  }

  void IrcServer::handle_message(std::string &msg_str) {
    m_server_event_handler.recieved_message(msg_str);
    IrcMessage msg(msg_str);
    auto it = m_msg_handlers.find(msg.command);
    if (it != m_msg_handlers.end()) {
      it->second(msg);
    }
  }

  void IrcServer::handle_connection_registration(IrcMessage &msg) {
    m_server_event_handler.connected();
  }

  void IrcServer::handle_nick_error(IrcMessage &msg) {
    m_out << IrcMessage("NICK", { m_nicks[++m_nick_id] }).str() << std::flush;
    m_server_event_handler.error(msg.trailing);    
  }
}
