#include "irc_channel_user.h"

namespace core {

  IrcChannelUser::IrcChannelUser(IrcUser &user, const std::string &channel_mode) :
    m_user(user),
    m_channel_mode(channel_mode.empty() ? ChanModeNone :
                   channel_mode[0] == '~' ? ChanModeFounder :
                   channel_mode[0] == '&' ? ChanModeProtected :
                   channel_mode[0] == '@' ? ChanModeOp :
                   channel_mode[0] == '%' ? ChanModeHalfOp :
                   channel_mode[0] == '+' ? ChanModeVoice :
                   ChanModeNone) {
  }

  IrcChannelUser::IrcChannelUser(const IrcChannelUser &user) :
    m_user(user.m_user),
    m_channel_mode(user.m_channel_mode) {
  }

  void IrcChannelUser::operator=(const IrcChannelUser &user) {
    m_user = user.m_user;
    m_channel_mode = user.m_channel_mode;
  }
  
  IrcUser &IrcChannelUser::user() {
    return m_user;
  }

  ChannelUserMode IrcChannelUser::channel_mode() {
    return m_channel_mode;
  }

}
