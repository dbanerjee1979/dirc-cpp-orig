#ifndef _TEXT_SERVER_EVENT_HANDLER_H
#define _TEXT_SERVER_EVENT_HANDLER_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <core/server_event_handler.h>
#include "server_run_loop.h"
#include "text_channel_event_handler.h"

namespace text {

  class TextServerEventHandler : public core::ServerEventHandler {
  public:
    TextServerEventHandler(const std::string &network, ServerRunLoop &server_run_loop);
    void recieved_message(const std::string &msg);
    void error(const std::string &msg);
    void connected();
    void handle_shutdown();
    TextChannelEventHandler *create_channel_event_handler(const std::string &channel);
  private:
    std::string m_network;
    ServerRunLoop &m_server_run_loop;
    std::vector<std::string> m_messages;
    int m_messages_unread;
    std::map<std::string, std::unique_ptr<TextChannelEventHandler>> m_channels;

    friend class App;
  };
    
}

#endif
