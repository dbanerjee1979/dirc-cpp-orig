#include "irc_message_commands.h"
#include "irc_entity_repository.h"

namespace core {

  IrcEntityRepository::IrcEntityRepository() {
  }

  void IrcEntityRepository::create_channel(std::string &channel,
                                           ChannelEventHandler *channel_handler) {
    auto ch = new IrcChannel(channel_handler);
    m_channels[channel] = std::unique_ptr<IrcChannel>(ch);
  }

  boost::optional<IrcChannel&> IrcEntityRepository::find_channel(IrcMessage &msg) {
    if (msg.command == RPL_TOPIC) {
      std::string &channel = msg.params[1];
      auto it = m_channels.find(channel);
      if (it != m_channels.end()) {
        return *(it->second);
      }
    }
    return boost::none;
  }

}
