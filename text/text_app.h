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
    boost::asio::ip::tcp::iostream stream;
    text::TextServerEventHandler server_event_handler;
    core::IrcServer server;
    std::thread server_thread;

    ServerHandle(config::Network &);
    static boost::asio::ip::tcp::iostream connect(config::Network &);
    void server_run_loop();
  };

  class App {
  public:
    App(std::vector<config::Network>);
    void run();
    void help();
  private:
    void with_network(std::string &, std::function<void(ServerHandle &)>);
    void connect_handler(std::stringstream &);
    void info_handler(std::stringstream &);
    void list_handler(std::stringstream &);
    
    std::vector<config::Network> m_networks;
    std::map<std::string, std::function<void(std::stringstream &)>> m_commands;
    std::map<std::string, std::unique_ptr<ServerHandle>> m_servers;
  };

}

#endif

