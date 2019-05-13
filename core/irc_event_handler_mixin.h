#ifndef _CORE_IRC_EVENT_HANDLER_MIXIN_H
#define _CORE_IRC_EVENT_HANDLER_MIXIN_H

#include <memory>
#include <functional>

namespace core {

  template <class EventType>
  class IrcEventHandlerMixin {
  public:
    // Add events to the head of the list, because events triggering destruction are the first events to get added
    void add_event_handler(std::shared_ptr<EventType> event_handler) {
      m_event_handlers.insert(m_event_handlers.begin(), event_handler);
    }
  protected:
    // Trigger events from last added to first added
    void send_event(std::function<void(EventType &)> handler) {
      for (auto it = m_event_handlers.begin(); it != m_event_handlers.end(); it++) {
        handler(**it);
      }
    }
  private:
    std::vector<std::shared_ptr<EventType>> m_event_handlers;
  };
  
}

#endif
