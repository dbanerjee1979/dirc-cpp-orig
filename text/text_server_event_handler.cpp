#include <iostream>
#include "text_server_event_handler.h"

namespace text {

  TextServerEventHandler::TextServerEventHandler(const std::string &network) :
    m_network(network),
    m_messages_unread(0) {
  }

  void TextServerEventHandler::recieved_message(std::string &msg) {
    m_messages.push_back(msg);
    m_messages_unread++;
  }

  void TextServerEventHandler::error(std::string &msg) {
    std::cout << "ERROR (" << m_network << ") " << msg << std::endl;
  }

  void TextServerEventHandler::connected() {
    std::cout << "Successfully connected to " << m_network << std::endl;
  }

}
