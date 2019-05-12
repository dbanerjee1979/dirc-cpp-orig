#ifndef _CORE_CHAT_HANDLER_H
#define _CORE_CHAT_HANDLER_H

#include <vector>
#include "irc_channel_user.h"

namespace core {

  class ChannelEventHandler {
  public:
    ChannelEventHandler() {
    }

    virtual ~ChannelEventHandler() {
    }

    virtual void disconnected() {
    }

    virtual void topic_changed(const std::string& msg) {
    }

    virtual void channel_users(const std::vector<IrcChannelUser *> &_users) {
    }

    virtual void user_quit(IrcChannelUser &_user, const std::string &msg) {
    }

    virtual void user_joined(IrcChannelUser &_user) {
    }

    virtual void user_departed(IrcChannelUser &_user, const std::string &msg) {
    }

    virtual void nick_changed(const std::string &nick_from, const std::string &nick_to) {
    }
  };
}

#endif
