#ifndef _TEXT_SERVER_EVENT_HANDLER_H
#define _TEXT_SERVER_EVENT_HANDLER_H

#include <vector>
#include <string>
#include <core/server_event_handler.h>
#include "server_run_loop.h"

namespace text {

  class TextServerEventHandler : public core::ServerEventHandler {
  public:
    TextServerEventHandler(const std::string &network, ServerRunLoop &server_run_loop);
    void recieved_message(const std::string &msg);
    void error(const std::string &msg);
    void connected();
    void handle_shutdown();
  private:
    std::string m_network;
    ServerRunLoop &m_server_run_loop;
    std::vector<std::string> m_messages;
    int m_messages_unread;

    friend class App;
  };
    
}

#endif
