#include <iostream>
#include "text_server_event_handler.h"

namespace text {

  void TextServerEventHandler::recieved_message(std::string &msg) {
    m_messages.push_back(msg);
    m_messages_unread++;
  }

}
