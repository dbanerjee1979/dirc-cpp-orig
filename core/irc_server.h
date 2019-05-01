#ifndef _CORE_IRC_SERVER_H
#define _CORE_IRC_SERVER_H

#include <string>
#include <iostream>
#include "server_event_handler.h"

namespace core {

  class IrcServer {
  public:
    IrcServer(std::ostream &, ServerEventHandler &);
    void handle_message(std::string &);
  private:
    std::ostream &m_out;
    ServerEventHandler &m_server_event_handler;
  };
  
}

#endif
