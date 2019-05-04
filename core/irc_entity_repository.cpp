#include "irc_entity_repository.h"

namespace core {

  IrcEntityRepository::IrcEntityRepository() {
  }

  void IrcEntityRepository::create_channel(std::string &channel,
                                           ChannelEventHandler *channel_handler) {
    auto ch = new IrcChannel(channel_handler);
    m_channels[channel] = std::unique_ptr<IrcChannel>(ch);
  }

}
