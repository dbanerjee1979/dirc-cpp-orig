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

namespace core {

  class IrcChannel {
  public:
    IrcChannel(const std::string &, std::ostream &, ChannelEventHandler *, IrcUserRepository &, std::function<void()>);
    std::string &name();
    void disconnect(const std::string &msg);
    void handle_message(const IrcMessage &);
  private:
    IrcChannelUser &add_user(const std::string &nick, const std::string &username, const std::string &chan_mode);
    void handle_topic(const IrcMessage &);
    void handle_no_topic(const IrcMessage &);
    void handle_name_reply(const IrcMessage &);
    void handle_name_reply_end(const IrcMessage &);
    void handle_quit(const IrcMessage &);
    void handle_join(const IrcMessage &);
    void handle_part(const IrcMessage &);

    std::string m_name;
    std::ostream &m_out;
    std::unique_ptr<ChannelEventHandler> m_event_handler;
    std::unordered_map<std::string, std::function<void(const IrcMessage &)>> m_msg_handlers;
    std::vector<std::unique_ptr<IrcChannelUser>> m_users;
    IrcUserRepository &m_user_repo;
    std::function<void()> m_disconnect_handler;
  };
  
}

#endif
