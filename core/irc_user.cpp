#include "irc_user.h"

namespace core {

  IrcUser::IrcUser(const std::string &nickname,
                   const std::string &username,
                   const std::string &realname) :
    m_nickname(nickname),
    m_username(username),
    m_realname(realname){
  }
  
  std::string &IrcUser::nickname() {
    return m_nickname;
  }

  std::string &IrcUser::username() {
    return m_username;
  }
  
  std::string &IrcUser::realname() {
    return m_realname;
  }

}
