#ifndef _CORE_IRC_MESSAGE_H
#define _CORE_IRC_MESSAGE_H

#include <string>
#include <vector>

namespace core {

  struct IrcMessage {
    std::string command;
    std::vector<std::string> params;
    std::string trailing;
    
    IrcMessage(const std::string &);
  };
  
}

#endif
