#include "text_channel_event_handler.h"

namespace text {

  void TextChannelEventHandler::topic_changed(const std::string& msg) {
    m_topic = msg;
  }

  void TextChannelEventHandler::channel_users(const std::vector<core::IrcChannelUser> &_users) {
    m_user_count = _users.size();
  }
  
}


