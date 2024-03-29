#ifndef _CORE_IRC_USER_REPO_H
#define _CORE_IRC_USER_REPO_H

#include <memory>
#include <boost/optional.hpp>
#include "irc_user.h"
#include "user_event_handler.h"
#include "chat_event_handler.h"

namespace core {

  class IrcUserRepository {
  public:
    virtual void create_user(const std::string &nickname, const std::string &username, const std::string &realname) = 0;
    virtual void remove_user(const std::string &nickname) = 0;
    virtual boost::optional<IrcUser&> find_user(const std::string &nickname) = 0;
  };
  
}

#endif

