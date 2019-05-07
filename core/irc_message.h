#ifndef _CORE_IRC_MESSAGE_H
#define _CORE_IRC_MESSAGE_H

#include <string>
#include <vector>

namespace core {

  struct IrcMessage {
    std::string servername;
    std::string nick;
    std::string user;
    std::string host;
    std::string command;
    std::vector<std::string> params;
    std::string trailing;
    
    IrcMessage(const std::string &);
    IrcMessage(const std::string &, const std::vector<std::string> &);
    IrcMessage(const std::string &, const std::vector<std::string> &, const std::string &);
    std::string str();
  };
  
}

#endif
