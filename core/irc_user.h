#ifndef _CORE_IRC_USER_H
#define _CORE_IRC_USER_H

#include <string>

namespace core {

  class IrcUser {
  public:
    IrcUser(const std::string &nickname, const std::string &username = "", const std::string &realname = "");
    const std::string &nickname();
    void nickname(const std::string &);
    const std::string &username();
    const std::string &realname();
  private:
    std::string m_nickname;
    std::string m_username;
    std::string m_realname;
  };

}

#endif
