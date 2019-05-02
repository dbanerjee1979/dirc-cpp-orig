#ifndef _CORE_CONFIG_H
#define _CORE_CONFIG_H

#include <string>
#include <vector>

namespace config {
  struct UserInfo {
    const std::vector<std::string> nicks;
    
    UserInfo(std::vector<std::string>);
  };
}

#endif
