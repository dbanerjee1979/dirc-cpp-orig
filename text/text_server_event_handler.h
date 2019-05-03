#ifndef _TEXT_SERVER_EVENT_HANDLER_H
#define _TEXT_SERVER_EVENT_HANDLER_H

#include <vector>
#include <string>
#include <core/server_event_handler.h>

namespace text {

  class TextServerEventHandler : public core::ServerEventHandler {
  public:
    void recieved_message(std::string &msg);
  private:
    std::vector<std::string> m_messages;
    int m_messages_unread;

    friend class App;
  };
    
}

#endif
