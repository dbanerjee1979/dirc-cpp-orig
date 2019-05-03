#include "text_app.h"

#include <string>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

namespace text {

  App::App(std::vector<config::Network> networks) :
    m_networks(networks) {
    using namespace std::placeholders;
    m_commands["connect"] = std::bind(&App::connect_handler, this, _1);
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
      auto server_cfg = it->servers.begin();
      boost::asio::ip::tcp::iostream ss(server_cfg->hostname, std::to_string(server_cfg->port));
      if (!ss) {
	std::cout << "Unable to connect: " << ss.error().message() << std::endl;
      }
    }
  }
  
}
