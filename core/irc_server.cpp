#include "irc_server.h"

namespace core {
  IrcServer::IrcServer(std::ostream &out,
		       ServerEventHandler &server_event_handler) :
    m_out(out),
    m_server_event_handler(server_event_handler) {
    m_out << "NICK nick\r" << std::endl;
    m_out << "USER foo 8 * :John\r" << std::endl;
  }

  void IrcServer::handle_message(std::string &msg) {
  }
  
}


  
