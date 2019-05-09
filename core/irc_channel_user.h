#ifndef _CORE_IRC_CHANNEL_USER_H
#define _CORE_IRC_CHANNEL_USER_H

#include <string>
#include "irc_user.h"

namespace core {

  class IrcChannelUser {
  public:
    IrcChannelUser(IrcUser &user, const std::string &channel_mode);
    IrcChannelUser(const IrcChannelUser &user);
    void operator=(const IrcChannelUser &user);
    IrcUser &user();
  private:
    IrcUser &m_user;
  };

}

#endif
