#ifndef _CORE_IRC_ENTITY_REPO_H
#define _CORE_IRC_ENTITY_REPO_H

#include <string>
#include <memory>
#include <unordered_map>
#include <iterator>
#include <boost/optional.hpp>
#include "irc_message.h"
#include "irc_channel.h"
#include "channel_event_handler.h"

namespace core {

  class IrcEntityRepository {
  public:
    IrcEntityRepository();
    void create_channel(std::string &channel, ChannelEventHandler *channel_handler);
    boost::optional<IrcChannel&> find_channel(IrcMessage &msg);
  private:
    std::unordered_map<std::string, std::unique_ptr<IrcChannel>> m_channels;
  };
  
}

#endif