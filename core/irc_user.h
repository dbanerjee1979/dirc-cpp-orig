#ifndef _CORE_IRC_USER_H
#define _CORE_IRC_USER_H

#include <string>

namespace core {

  class IrcUser {
  public:
    IrcUser(const std::string &nickname);
    std::string &nickname();
  private:
    std::string m_nickname;
  };

}

#endif
