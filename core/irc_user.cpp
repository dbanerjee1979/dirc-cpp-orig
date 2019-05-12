#include "irc_user.h"

namespace core {

  IrcUser::IrcUser(const std::string &nickname,
                   const std::string &username,
                   const std::string &realname) :
    m_nickname(nickname),
    m_username(username),
    m_realname(realname){
  }
  
  const std::string &IrcUser::nickname() {
    return m_nickname;
  }

  void IrcUser::nickname(const std::string &nickname) {
    m_nickname = nickname;
  }

  const std::string &IrcUser::username() {
    return m_username;
  }
  
  const std::string &IrcUser::realname() {
    return m_realname;
  }

}
