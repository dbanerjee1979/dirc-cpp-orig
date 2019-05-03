#ifndef _CORE_SERVER_HANDLER_H
#define _CORE_SERVER_HANDLER_H

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

    virtual void notice(std::string &recipient, std::string &msg) {
    }

    virtual void message(std::string &msg) {
    }
  };
    
}

#endif
