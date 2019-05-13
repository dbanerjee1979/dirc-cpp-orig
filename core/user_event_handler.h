#ifndef _CORE_USER_HANDLER_H
#define _CORE_USER_HANDLER_H

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
