#ifndef _CORE_CHAT_HANDLER_H
#define _CORE_CHAT_HANDLER_H

namespace core {

  class ChannelEventHandler {
  public:
    ChannelEventHandler() {
    }

    virtual ~ChannelEventHandler() {
    }

    virtual void topic_changed(std::string& msg) {
    }
  };
}

#endif
