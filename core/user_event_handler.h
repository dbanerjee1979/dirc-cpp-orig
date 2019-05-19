#ifndef _CORE_USER_HANDLER_H
#define _CORE_USER_HANDLER_H

#include <memory>
#include "irc_user.h"

namespace core {

  class UserEventHandler {
  public:
    virtual void nick_changed(const std::string &nick_from, const std::string &nick_to) {
    }

    virtual void quit(const std::string &msg) {
    }
  };
  
}

#endif
