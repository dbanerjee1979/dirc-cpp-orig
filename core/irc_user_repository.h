#ifndef _CORE_IRC_USER_REPO_H
#define _CORE_IRC_USER_REPO_H

#include <boost/optional.hpp>
#include "irc_user.h"

namespace core {

  class IrcUserRepository {
  public:
    virtual boost::optional<IrcUser&> find_user(const std::string &nickname) = 0;
  };
  
}

#endif

