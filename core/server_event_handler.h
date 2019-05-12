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

    virtual void recieved_message(const std::string &msg) {
    }

    virtual void error(const std::string &msg) {
    }

    virtual void connected() {
    }

    virtual void handle_shutdown() {
    }

    virtual ChannelEventHandler *create_channel_event_handler(const std::string &channel) {
      return NULL;
    }

    virtual void notice(const std::string &recipient, const std::string &msg) {
    }

    virtual void message(const std::string &msg) {
    }

    virtual void message_of_the_day(const std::string &msg) {
    }

    virtual void user_quit(IrcUser &_user, const std::string &msg) {
    }

    virtual void nick_changed(const std::string &nick_from, const std::string &nick_to) {
    }
  };
    
}

#endif
