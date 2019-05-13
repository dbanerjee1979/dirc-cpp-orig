#ifndef _CORE_IRC_USER_H
#define _CORE_IRC_USER_H

#include <string>
#include <unordered_map>
#include <functional>
#include "irc_message.h"
#include "irc_message_handler_mixin.h"
#include "irc_event_handler_mixin.h"
#include "user_event_handler.h"

namespace core {

  class IrcUser : public IrcMessageHandlerMixin, public IrcEventHandlerMixin<UserEventHandler> {
  public:
    IrcUser(const std::string &nickname, const std::string &username = "", const std::string &realname = "");
    const std::string &nickname();
    void nickname(const std::string &);
    const std::string &username();
    const std::string &realname();
  private:
    void handle_nick(const IrcMessage &);
    void handle_quit(const IrcMessage &);
    
    std::string m_nickname;
    std::string m_username;
    std::string m_realname;
  };

}

#endif
