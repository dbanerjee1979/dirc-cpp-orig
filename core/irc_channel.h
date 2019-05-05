#ifndef _CORE_IRC_CHANNEL_H
#define _CORE_IRC_CHANNEL_H

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "irc_message.h"
#include "channel_event_handler.h"

namespace core {

  class IrcChannel {
  public:
    IrcChannel(ChannelEventHandler *event_handler);
    void handle_message(IrcMessage &);
  private:
    void handle_topic(IrcMessage &);
    
    std::unique_ptr<ChannelEventHandler> m_event_handler;
    std::unordered_map<std::string, std::function<void(IrcMessage&)>> m_msg_handlers;
  };
  
}

#endif
