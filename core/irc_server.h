#ifndef _CORE_IRC_SERVER_H
#define _CORE_IRC_SERVER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "server_event_handler.h"
#include "irc_message.h"
#include "config.h"

namespace core {

  class IrcServer {
  public:
    IrcServer(config::UserInfo &, config::Network &, std::ostream &, ServerEventHandler &);
    void handle_message(std::string &);
  private:
    void handle_connection_registration(IrcMessage &);

    std::ostream &m_out;
    ServerEventHandler &m_server_event_handler;
    std::unordered_map<std::string, std::function<void(IrcMessage&)>> m_msg_handlers;
  };
  
}

#endif
