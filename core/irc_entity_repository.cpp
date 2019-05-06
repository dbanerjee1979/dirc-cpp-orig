#include "irc_message_commands.h"
#include "irc_entity_repository.h"

namespace core {

  IrcEntityRepository::IrcEntityRepository() {
    m_entity_idx[RPL_TOPIC]      = 1;
    m_entity_idx[RPL_NOTOPIC]    = 1;
    m_entity_idx[RPL_NAMREPLY]   = 2;
    m_entity_idx[RPL_ENDOFNAMES] = 1;
  }

  void IrcEntityRepository::create_channel(std::string &channel,
                                           ChannelEventHandler *channel_handler) {
    auto ch = new IrcChannel(channel, channel_handler);
    m_channels[channel] = std::unique_ptr<IrcChannel>(ch);
  }

  boost::optional<IrcChannel&> IrcEntityRepository::find_channel(IrcMessage &msg) {
    auto it = m_entity_idx.find(msg.command);
    int idx;
    if (it != m_entity_idx.end() && (msg.params.size() > (idx = it->second))) {
      std::string &channel = msg.params[idx];
      auto it = m_channels.find(channel);
      if (it != m_channels.end()) {
        return *(it->second);
      }
    }
    return boost::none;
  }

}
