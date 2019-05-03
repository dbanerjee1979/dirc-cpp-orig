#ifndef _TEXT_APP_H
#define _TEXT_APP_H

#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <core/config.h>
#include <core/irc_server.h>
#include "text_server_event_handler.h"

namespace text {

  struct ServerHandle {
    std::unique_ptr<boost::asio::ip::tcp::iostream> stream;
    std::unique_ptr<text::TextServerEventHandler> server_event_handler;
    std::unique_ptr<core::IrcServer> server;
    std::thread server_thread;

    void server_run_loop();
  };

  class App {
  public:
    App(std::vector<config::Network>);
    void run();
    void help();
  private:
    void connect_handler(std::stringstream &);
    void info_handler(std::stringstream &);
    
    std::vector<config::Network> m_networks;
    std::map<std::string, std::function<void(std::stringstream &)>> m_commands;
    std::map<std::string, ServerHandle> m_servers;
  };

}

#endif
