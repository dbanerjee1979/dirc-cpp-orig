#ifndef text_server_run_loop
#define text_server_run_loop

#include <mutex>
#include <core/irc_server.h>

namespace text {

  class ServerRunLoop {
  public:
    ServerRunLoop(std::istream &stream);
    void set_server(core::IrcServer *server);
    void run();
    void shutdown();
  private:
    std::istream &m_stream;
    core::IrcServer *m_server;
    std::mutex m_mutex;
    bool m_running;
  };
  
}

#endif
