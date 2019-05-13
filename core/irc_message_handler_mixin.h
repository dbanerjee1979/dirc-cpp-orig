#ifndef _CORE_IRC_MESSAGE_HANDLER_MIXIN_H
#define _CORE_IRC_MESSAGE_HANDLER_MIXIN_H

#include <unordered_map>
#include <functional>
#include "irc_message.h"

namespace core {

  class IrcMessageHandlerMixin {
  public:
    void handle_message(const IrcMessage &);
  protected:
    std::unordered_map<std::string, std::function<void(const IrcMessage &)>> m_msg_handlers;
  };
  
}

#endif
