#ifndef _CORE_IRC_ENTITY_REPO_H
#define _CORE_IRC_ENTITY_REPO_H

#include <string>
#include <memory>
#include <unordered_map>
#include <iterator>
#include <boost/optional.hpp>
#include "irc_message.h"
#include "irc_channel.h"
#include "irc_user.h"
#include "channel_event_handler.h"
#include "irc_user_repository.h"

namespace core {

  class IrcEntityRepository : public IrcUserRepository {
  public:
    IrcEntityRepository();
    void create_channel(std::string &channel, ChannelEventHandler *channel_handler);
    boost::optional<IrcChannel&> find_channel(IrcMessage &msg);
    boost::optional<IrcUser&> find_user(const std::string &nickname);
  private:
    std::unordered_map<std::string, std::unique_ptr<IrcChannel>> m_channels;
    std::unordered_map<std::string, int> m_entity_idx;
  };
  
}

#endif

