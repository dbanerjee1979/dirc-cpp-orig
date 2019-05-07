#ifndef _CORE_IRC_CHANNEL_H
#define _CORE_IRC_CHANNEL_H

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <boost/algorithm/string.hpp>
#include "irc_user.h"
#include "irc_message.h"
#include "channel_event_handler.h"

namespace core {

  class IrcChannel {
  public:
    IrcChannel(std::string &name, ChannelEventHandler *event_handler);
    std::string &name();
    void handle_message(IrcMessage &);
  private:
    void handle_topic(IrcMessage &);
    void handle_no_topic(IrcMessage &);
    void handle_name_reply(IrcMessage &);
    void handle_name_reply_end(IrcMessage &);

    std::string m_name;
    std::unique_ptr<ChannelEventHandler> m_event_handler;
    std::unordered_map<std::string, std::function<void(IrcMessage&)>> m_msg_handlers;
    std::vector<IrcUser> m_users;
  };
  
}

#endif
