#ifndef _TEXT_APP_H
#define _TEXT_APP_H

#include <vector>
#include <core/config.h>

namespace text {

  class App {
  public:
    App(std::vector<config::Network>);
    void run();
  private:
    std::vector<config::Network> m_networks;
  };

}

#endif
