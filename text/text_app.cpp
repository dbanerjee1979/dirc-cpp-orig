#include "text_app.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>

namespace text {

  App::App(std::vector<config::Network> networks) :
    m_networks(networks) {
    using namespace std::placeholders;
    m_commands["connect"] = std::bind(&App::connect_handler, this, _1);
    m_commands["info"] = std::bind(&App::info_handler, this, _1);
    m_commands["list"] = std::bind(&App::list_handler, this, _1);
    m_commands["join"] = std::bind(&App::join_handler, this, _1);
  }
  
  void App::run() {
    std::string cmd_line;
    std::string cmd;
    for(;;) {
      std::cout << "> ";
      getline(std::cin, cmd_line);
      
      std::stringstream ss(cmd_line);
      std::getline(ss, cmd, ' ');
      boost::algorithm::to_lower(cmd);

      if (cmd == "quit") {
        break;
      }
      else if (cmd == "help") {
        help();
      }
      else {
        auto it = m_commands.find(cmd);
        if (it != m_commands.end()) {
          it->second(ss);
        }
        else {
          std::cout << "Unknown command: " << cmd << std::endl;
          help();
        }
      }
    }
  }

  void App::help() {
    std::cout << "Available commands:" << std::endl;
    for(auto it = m_commands.begin(); it != m_commands.end(); ++it) {
      std::cout << "  " << it->first << std::endl;
    }
    std::cout << "  quit" << std::endl;
  }

  void App::with_network(std::string& network, std::function<void(ServerHandle&)> action) {
    boost::to_lower(network);
    auto it = m_servers.find(network);
    if (it == m_servers.end()) {
      std::cout << "Unknown network" << std::endl;
    }
    else {
      action(*(it->second));
    }
  }

  ServerHandle::ServerHandle(config::Network &network) :
    stream(ServerHandle::connect(network)),
    server_event_handler(text::TextServerEventHandler(network.name)),
    server(network, stream, server_event_handler, entity_repo) {
    server_thread = std::thread(&ServerHandle::server_run_loop, this);
    server_thread.detach();
  }

  boost::asio::ip::tcp::iostream ServerHandle::connect(config::Network &network) {
    auto server_cfg = network.servers.begin();
    auto ss = boost::asio::ip::tcp::iostream(server_cfg->hostname, std::to_string(server_cfg->port));
    if (!ss) {
      throw std::runtime_error("Unable to connect: " + ss.error().message());
    }
    return ss;
  }

  void ServerHandle::server_run_loop() {
    std::string line;
    while (!stream.eof()) {
      getline(stream, line);
      server.handle_message(line);
    }
  }
  
  void App::connect_handler(std::stringstream &ss) {
    std::string network;
    std::getline(ss, network, ' ');

    if (network.empty()) {
      std::cout << "Usage: connect <network>" << std::endl;
    }

    boost::algorithm::to_lower(network);
    auto it = m_networks.begin();
    for (; it != m_networks.end(); ++it) {
      if (boost::iequals(it->name, network)) {
        break;
      }
    }

    if (it == m_networks.end()) {
      std::cout << "Unknown network, choose from:" << std::endl;
      for (auto it = m_networks.begin(); it != m_networks.end(); ++it) {
        std::cout << "  " << it->name << std::endl;
      }
    }
    else {
      try {
        std::cout << "Connecting to " << it->name << "..." << std::endl;
        std::string key(it->name);
        boost::algorithm::to_lower(key);
        m_servers[key] = std::unique_ptr<ServerHandle>(new ServerHandle(*it));
      }
      catch (std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
      }
    }
  }

  void App::info_handler(std::stringstream &ss) {
    std::cout << "Networks:" << std::endl;
    for (auto it = m_servers.begin(); it != m_servers.end(); ++it) {
      std::cout << "  " << it->first << ":" << std::endl;

      TextServerEventHandler &sh = it->second->server_event_handler;

      std::cout << "    log ";
      if (sh.m_messages_unread > 0) {
        std::cout << " (" << sh.m_messages_unread << ")";
      }
      std::cout << std::endl;
    }
  }

  void App::list_handler(std::stringstream &ss) {
    std::string network;
    std::string entity;
    std::string nick;

    std::getline(ss, network, ' ');
    std::getline(ss, entity, ' ');
    std::getline(ss, nick, ' ');

    bool log = false;
    bool messages = false;
    bool chat = false;
    if (network.empty() || (
            !(log = boost::iequals(entity, "log")) &&
            !(messages = boost::iequals(entity, "messages")) &&
            !(chat = boost::iequals(entity, "chat") && !nick.empty()))) {
      std::cout << "Usage: list <network> log|messages|chat <nick>" << std::endl;
      return;
    }

    
    with_network(network, [nick, log, messages, chat, this](ServerHandle& h) {
      TextServerEventHandler &sh = h.server_event_handler;

      if (log) {
        sh.m_messages_unread = 0;
        for (auto lit = sh.m_messages.begin(); lit != sh.m_messages.end(); ++lit) {
          std::cout << *lit << std::endl;
        }
      }
    });
  }

  void App::join_handler(std::stringstream &ss) {
    std::string network;
    std::string channel;

    std::getline(ss, network, ' ');
    std::getline(ss, channel);

    if (network.empty() || channel.empty()) {
      std::cout << "Usage: join <network> <channel>" << std::endl;
      return;
    }

    with_network(network, [channel, this](ServerHandle& h) {
      h.server.join(channel);      
    });
  }
}
