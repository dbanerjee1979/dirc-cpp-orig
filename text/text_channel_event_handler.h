#ifndef _TEXT_CHANNEL_EVENT_HANDLER_H
#define _TEXT_CHANNEL_EVENT_HANDLER_H

#include <core/channel_event_handler.h>

namespace text {

  class TextChannelEventHandler : public core::ChannelEventHandler {
  public:
    void topic_changed(const std::string& msg);
    void channel_users(core::ChannelUserIterator begin, core::ChannelUserIterator end);
  private:
    std::string m_topic;
    int m_user_count;

    friend class App;
  };
  
}

#endif
