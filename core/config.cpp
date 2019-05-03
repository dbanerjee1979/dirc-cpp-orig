#include "config.h"

namespace config {

  UserInfo::UserInfo(std::vector<std::string> _nicks,
		     std::string _username,
		     std::string _realname) :
    nicks(_nicks),
    username(_username),
    realname(_realname) {
  }

  Network::Network(std::string _name, UserInfo _user_info) :
    name(_name),
    user_info(_user_info) {
  }

}
