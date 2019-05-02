#ifndef _CORE_SERVER_HANDLER_H
#define _CORE_SERVER_HANDLER_H

namespace core {
  
  class ServerEventHandler {
  public:
    ServerEventHandler() {
    }

    virtual ~ServerEventHandler() {
    }

    virtual void connected() {
    }
  };
    
}

#endif
