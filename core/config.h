#ifndef _CORE_CONFIG_H
#define _CORE_CONFIG_H

#include <string>
#include <vector>

namespace config {
  struct UserInfo {
    const std::vector<std::string> nicks;
    const std::string username;
    const std::string realname;
    const std::string password;
    
    UserInfo(std::vector<std::string>, std::string, std::string, std::string);
  };

  struct Server {
    const std::string hostname;
    const int port;

    Server(std::string, int);
  };

  struct Network {
    const std::string name;
    const std::vector<Server> servers;
    const UserInfo user_info;

    Network(std::string, std::vector<Server>, UserInfo);
  };
}

#endif
