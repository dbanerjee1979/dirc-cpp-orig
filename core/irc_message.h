#ifndef _CORE_IRC_MESSAGE_H
#define _CORE_IRC_MESSAGE_H

#include <string>
#include <vector>

namespace core {

  class IrcMessage {
  public:
    IrcMessage(const std::string &);
    IrcMessage(const std::string &, const std::vector<std::string> &);
    IrcMessage(const std::string &, const std::vector<std::string> &, const std::string &);
    std::string str();
    const std::string &servername() const;
    const std::string &nick() const;
    const std::string &user() const;
    const std::string &host() const;
    const std::string &command() const;
    const std::string &trailing() const;
    const int param_count();
    const std::string &param(int) const;
    std::string param_str() const;
  private:
    std::string m_servername;
    std::string m_nick;
    std::string m_user;
    std::string m_host;
    std::string m_command;
    std::vector<std::string> m_params;
    std::string m_trailing;
  };
  
}

#endif
