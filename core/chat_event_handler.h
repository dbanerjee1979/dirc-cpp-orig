#ifndef _CORE_CHAT_HANDLER_H
#define _CORE_CHAT_HANDLER_H

namespace core {

  class ChatEventHandler {
  public:
    virtual ~ChatEventHandler() {
    }
    
    virtual void recieved_message(const std::string &msg) {
    }
  };

  class ChatEventHandlerFactory {
  public:
    virtual ~ChatEventHandlerFactory() {
    }
    
    virtual std::shared_ptr<ChatEventHandler> create_chat_event_handler() {
      return std::shared_ptr<ChatEventHandler>();
    }
  };
  
}

#endif
