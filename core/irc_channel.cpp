#include "irc_channel.h"

namespace core {

  IrcChannel::IrcChannel(ChannelEventHandler *event_handler) :
    m_event_handler(event_handler) {
  }

}
