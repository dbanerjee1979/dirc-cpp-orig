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
#include "irc_user_repository.h"
#include "irc_message_handler_mixin.h"
#include "irc_event_handler_mixin.h"

namespace core {

  class IrcChannel : public IrcMessageHandlerMixin, public IrcEventHandlerMixin<ChannelEventHandler> {
  public:
    IrcChannel(const std::string &, std::ostream &, std::shared_ptr<ChannelEventHandler>, IrcUserRepository &);
    std::string &name();
    void disconnect(const std::string &msg);
  private:
    IrcChannelUser &add_user(const std::string &nick, const std::string &username, const std::string &chan_mode);
    void handle_topic(const IrcMessage &);
    void handle_no_topic(const IrcMessage &);
    void handle_name_reply(const IrcMessage &);
    void handle_name_reply_end(const IrcMessage &);
    void handle_join(const IrcMessage &);
    void handle_part(const IrcMessage &);
    void handle_nick(const IrcMessage &);

    std::string m_name;
    std::ostream &m_out;

    std::vector<IrcChannelUser> m_users;
    IrcUserRepository &m_user_repo;
  };
  
}

#endif
