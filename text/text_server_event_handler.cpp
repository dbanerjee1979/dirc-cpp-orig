#include <iostream>
#include "text_server_event_handler.h"

namespace text {

  void TextServerEventHandler::recieved_message(std::string &msg) {
    std::cout << msg << std::endl;
  }

}
