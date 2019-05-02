#include "irc_server.h"
#include "irc_server_commands.h"

namespace core {
  IrcServer::IrcServer(config::UserInfo &user_info,
		       config::Network &network,
		       std::ostream &out,
		       ServerEventHandler &server_event_handler) :
    m_out(out),
    m_server_event_handler(server_event_handler) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_WELCOME] = std::bind(&IrcServer::handle_connection_registration, *this, _1);
    
    m_out << "NICK " << user_info.nicks[0] << "\r" << std::endl;
    m_out << "USER " << network.username << " 8 * :" << network.realname << "\r" << std::endl;
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


  
