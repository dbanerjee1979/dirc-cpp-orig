#ifndef _CORE_IRC_CHANNEL_H
#define _CORE_IRC_CHANNEL_H

#include <string>
#include <memory>
#include "channel_event_handler.h"

namespace core {

  class IrcChannel {
  public:
    IrcChannel(ChannelEventHandler *event_handler);
  private:
    std::unique_ptr<ChannelEventHandler> m_event_handler;
  };
  
}

#endif
