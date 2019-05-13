#include "irc_message_handler_mixin.h"

namespace core {

  void IrcMessageHandlerMixin::handle_message(const IrcMessage &msg) {
    auto it = m_msg_handlers.find(msg.command());
    if (it != m_msg_handlers.end()) {
      it->second(msg);
    }
  }

}
