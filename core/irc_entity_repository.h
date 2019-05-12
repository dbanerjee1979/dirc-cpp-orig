#ifndef _CORE_IRC_ENTITY_REPO_H
#define _CORE_IRC_ENTITY_REPO_H

#include <string>
#include <memory>
#include <unordered_map>
#include <iterator>
#include <functional>
#include <boost/optional.hpp>
#include "irc_message.h"
#include "irc_channel.h"
#include "channel_event_handler.h"
#include "irc_user_repository.h"

namespace core {

  class IrcEntityRepository : public IrcUserRepository {
  public:
    IrcEntityRepository();
    void create_channel(const std::string &channel, std::ostream &out, std::shared_ptr<ChannelEventHandler> channel_handler);
    boost::optional<IrcChannel &> find_channel(const IrcMessage &msg);
    void foreach_channels(std::function<void(IrcChannel &)> handler);
    void create_user(const std::string &nickname, const std::string &username = "", const std::string &realname = "");
    void remove_user(const std::string &nickname);
    void change_nick(const std::string &nickname_from, const std::string &nickname_to);
    boost::optional<IrcUser &> find_user(const IrcMessage &msg);
    boost::optional<IrcUser &> find_user(const std::string &nickname);
  private:
    std::unordered_map<std::string, std::unique_ptr<IrcChannel>> m_channels;
    std::unordered_map<std::string, std::unique_ptr<IrcUser>> m_users;
    std::unordered_map<std::string, int> m_entity_idx;
  };
  
}

#endif

