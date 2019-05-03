#ifndef _TEXT_APP_H
#define _TEXT_APP_H

#include <vector>
#include <map>
#include <functional>
#include <core/config.h>

namespace text {

  class App {
  public:
    App(std::vector<config::Network>);
    void run();
    void help();
  private:
    void connect_handler(std::stringstream &);
    
    std::vector<config::Network> m_networks;
    std::map<std::string, std::function<void(std::stringstream &)>> m_commands;
  };

}

#endif
