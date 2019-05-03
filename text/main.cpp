#include <iostream>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <core/config.h>
#include "text_app.h"

int main(int argc, char **argv) {
  text::App app({
      config::Network(
        "freenode",
        {
         config::Server("irc.freenode.net", 8001)
        },
        config::UserInfo(
          { "shorugoru", "_shorugoru_", "__shorugoru__" },
          "duke",
          "Duke"
        )
      )
    });
  app.run();
  return 0;
}
