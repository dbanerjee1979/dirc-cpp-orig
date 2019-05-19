#ifndef _CORE_IRC_CHANNEL_USER_H
#define _CORE_IRC_CHANNEL_USER_H

#include <string>
#include "irc_user.h"

namespace core {

  enum ChannelUserMode { ChanModeNone, ChanModeFounder, ChanModeProtected, ChanModeOp, ChanModeHalfOp, ChanModeVoice };

  class IrcChannelUser {
  public:
    IrcChannelUser(IrcUser &user, const std::string &channel_mode);
    IrcUser &user();
    ChannelUserMode channel_mode();
  private:
    IrcUser &m_user;
    ChannelUserMode m_channel_mode;
  };

}

#endif
