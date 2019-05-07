#ifndef _CORE_CHAT_HANDLER_H
#define _CORE_CHAT_HANDLER_H

#include <vector>
#include "irc_user.h"

namespace core {

  class ChannelEventHandler {
  public:
    ChannelEventHandler() {
    }

    virtual ~ChannelEventHandler() {
    }

    virtual void topic_changed(const std::string& msg) {
    }

    virtual void channel_users(const std::vector<IrcUser *> &_users) {
    }
  };
}

#endif
