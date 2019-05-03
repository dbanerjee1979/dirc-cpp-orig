#ifndef _CORE_CONFIG_H
#define _CORE_CONFIG_H

#include <string>
#include <vector>

namespace config {
  struct UserInfo {
    const std::vector<std::string> nicks;
    const std::string username;
    const std::string realname;
    
    UserInfo(std::vector<std::string>, std::string, std::string);
  };

  struct Network {
    const std::string name;
    const UserInfo user_info;

    Network(std::string, UserInfo);
  };
}

#endif
