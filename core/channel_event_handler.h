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

    virtual void topic_changed(std::string& msg) {
    }

    virtual void channel_users(std::vector<IrcUser *> &_users) {
    }
  };
}

#endif
