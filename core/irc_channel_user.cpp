#include "irc_channel_user.h"

namespace core {

  IrcChannelUser::IrcChannelUser(IrcUser &user, const std::string &channel_mode) :
    m_user(user) {
  }

  IrcChannelUser::IrcChannelUser(const IrcChannelUser &user) :
    m_user(user.m_user) {
  }

  void IrcChannelUser::operator=(const IrcChannelUser &user) {
    m_user = user.m_user;
  }
  
  IrcUser &IrcChannelUser::user() {
    return m_user;
  }

}
