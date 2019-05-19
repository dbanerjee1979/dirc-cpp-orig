#ifndef _CORE_IRC_USER_H
#define _CORE_IRC_USER_H

#include <string>
#include <unordered_map>
#include <functional>
#include "irc_message.h"
#include "irc_message_handler_mixin.h"
#include "irc_event_handler_mixin.h"
#include "user_event_handler.h"
#include "chat_event_handler.h"

namespace core {

  class IrcUser : public IrcMessageHandlerMixin,
                  public IrcEventHandlerMixin<UserEventHandler>,
                  public IrcEventHandlerMixin<ChatEventHandler>{
  public:
    IrcUser(const std::string &nickname,
            const std::string &username,
            const std::string &realname,
            const std::shared_ptr<ChatEventHandlerFactory> chat_factory);
    const std::string &nickname();
    void nickname(const std::string &);
    const std::string &username();
    const std::string &realname();
    bool is_client();
    void add_user_event_handler(std::shared_ptr<UserEventHandler>);
    void add_chat_event_handler(std::shared_ptr<ChatEventHandler>);
    
  private:
    void handle_nick(const IrcMessage &);
    void handle_quit(const IrcMessage &);
    void handle_private_chat(const IrcMessage &);

    bool m_is_client;
    std::string m_nickname;
    std::string m_username;
    std::string m_realname;
    std::shared_ptr<ChatEventHandlerFactory> m_chat_factory;
    std::shared_ptr<ChatEventHandler> m_chat;
  };

}

#endif
