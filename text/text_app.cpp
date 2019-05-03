#include "text_app.h"

#include <string>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace text {

  App::App(std::vector<config::Network> networks) :
    m_networks(networks) {
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
    }
  }

  void App::help() {
    std::cout << "Available commands:" << std::endl;
    for(auto it = m_commands.begin(); it != m_commands.end(); ++it) {
      std::cout << "  " << it->first << std::endl;
    }
    std::cout << "  quit" << std::endl;
  }
  
}
