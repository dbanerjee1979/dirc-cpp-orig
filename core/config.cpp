#include "config.h"

namespace config {

  UserInfo::UserInfo(std::vector<std::string> _nicks) :
    nicks(_nicks) {
  }

  Network::Network(std::string _username, std::string _realname) :
    username(_username),
    realname(_realname) {
  }

}
