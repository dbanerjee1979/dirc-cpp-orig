#include <iostream>
#include <core/irc_server.h>
#include "text_server_event_handler.h"

namespace text {

  TextServerEventHandler::TextServerEventHandler(const std::string &network,
                                                 ServerRunLoop &server_run_loop) :
    m_network(network),
    m_server_run_loop(server_run_loop),
    m_messages_unread(0) {
  }

  void TextServerEventHandler::recieved_message(const std::string &msg) {
    m_messages.push_back(msg);
    m_messages_unread++;
  }

  void TextServerEventHandler::error(const std::string &msg) {
    std::cout << "ERROR (" << m_network << ") " << msg << std::endl;
  }

  void TextServerEventHandler::connected() {
    std::cout << "Successfully connected to " << m_network << std::endl;
  }

  void TextServerEventHandler::handle_shutdown() {
    m_server_run_loop.shutdown();
  }
  
}
