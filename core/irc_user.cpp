#include "irc_user.h"

namespace core {

  IrcUser::IrcUser(const std::string &nickname) :
    m_nickname(nickname) {
  }
  
  std::string &IrcUser::nickname() {
    return m_nickname;
  }

}
