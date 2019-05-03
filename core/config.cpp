#include "config.h"

namespace config {

  UserInfo::UserInfo(std::vector<std::string> _nicks,
		     std::string _username,
		     std::string _realname) :
    nicks(_nicks),
    username(_username),
    realname(_realname) {
  }

  Server::Server(std::string _hostname, int _port) :
    hostname(_hostname),
    port(_port) {
  }

  Network::Network(std::string _name,
		   std::vector<Server> _servers,
		   UserInfo _user_info) :
    name(_name),
    servers(_servers),
    user_info(_user_info) {
  }

}
