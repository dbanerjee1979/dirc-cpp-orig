#include <iostream>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include "text_app.h"

int main(int argc, char **argv) {
  /*
  boost::asio::ip::tcp::iostream s("irc.freenode.net", "8001");

  std::istream &in = s;
  std::ostream &out = s;

  out << "NICK _shorugoru_\r\n";
  out << "USER duke 8 * :Duke\r\n";
  
  std::string line;
  while (!in.eof()) {
    getline(in, line);
    std::cout << line << std::endl;
  }
  */

  text::App app;
  app.run();
  return 0;
}
