#ifndef _TEXT_SERVER_EVENT_HANDLER_H
#define _TEXT_SERVER_EVENT_HANDLER_H

#include <vector>
#include <string>
#include <core/server_event_handler.h>

namespace text {

  class TextServerEventHandler : public core::ServerEventHandler {
  public:
    TextServerEventHandler(const std::string &network);
    void recieved_message(std::string &msg);
    void connected();
  private:
    std::string m_network;
    std::vector<std::string> m_messages;
    int m_messages_unread;

    friend class App;
  };
    
}

#endif
