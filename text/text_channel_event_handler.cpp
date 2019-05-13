#include "text_channel_event_handler.h"

namespace text {

  void TextChannelEventHandler::topic_changed(const std::string& msg) {
    m_topic = msg;
  }

  void TextChannelEventHandler::channel_users(core::ChannelUserIterator begin, core::ChannelUserIterator end) {
    m_user_count = std::distance(begin, end);
  }
  
}


