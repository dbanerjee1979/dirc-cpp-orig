#include "irc_server.h"
#include "irc_server_commands.h"

namespace core {
  IrcServer::IrcServer(config::Network &network,
		       std::ostream &out,
		       ServerEventHandler &server_event_handler) :
    m_out(out),
    m_server_event_handler(server_event_handler) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_WELCOME] = std::bind(&IrcServer::handle_connection_registration, *this, _1);

    if (!network.user_info.password.empty()) {
      m_out << "PASSWORD " << network.user_info.password << "\r" << std::endl;
    }
    m_out << "NICK " << network.user_info.nicks[0] << "\r" << std::endl;
    m_out << "USER " << network.user_info.username << " 8 * :" << network.user_info.realname << "\r" << std::endl;
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
}


  
