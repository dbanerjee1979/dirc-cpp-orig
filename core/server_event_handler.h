#ifndef _CORE_SERVER_HANDLER_H
#define _CORE_SERVER_HANDLER_H

#include "channel_event_handler.h"

namespace core {
  
  class ServerEventHandler {
  public:
    ServerEventHandler() {
    }

    virtual ~ServerEventHandler() {
    }

    virtual void recieved_message(std::string &msg) {
    }

    virtual void error(std::string &msg) {
    }

    virtual void connected() {
    }

    virtual void handle_shutdown() {
    }

    virtual ChannelEventHandler *create_channel_event_handler(std::string &channel) {
      return NULL;
    }

    virtual void notice(std::string &recipient, std::string &msg) {
    }

    virtual void message(std::string &msg) {
    }

    virtual void message_of_the_day(std::string &msg) {
    }
  };
    
}

#endif
