#ifndef _CORE_IRC_SERVER_H
#define _CORE_IRC_SERVER_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <sstream>
#include "server_event_handler.h"
#include "channel_event_handler.h"
#include "irc_message.h"
#include "irc_entity_repository.h"
#include "config.h"

namespace core {

  class IrcServer {
  public:
    IrcServer(config::Network &, std::ostream &, ServerEventHandler &, IrcEntityRepository &);
    void shutdown();
    void quit(const std::string &);
    void join(const std::string &);
    void nick(const std::string &);
    void handle_message(const std::string &);
  private:
    const std::string &nick();
    void handle_message_default(const IrcMessage &);
    void handle_connection_registration(const IrcMessage &);
    void handle_nick_error(const IrcMessage &);
    void handle_notice(const IrcMessage &);
    void handle_motd_start(const IrcMessage &);
    void handle_motd(const IrcMessage &);
    void handle_motd_end(const IrcMessage &);
    void handle_ping(const IrcMessage &);
    void handle_join(const IrcMessage &);
    void handle_quit(const IrcMessage &);

    std::ostream &m_out;
    ServerEventHandler &m_event_handler;
    std::unordered_map<std::string, std::function<void(const IrcMessage &)>> m_msg_handlers;
    const config::UserInfo &m_user_info;
    int m_nick_id;
    std::stringstream m_motd;
    IrcEntityRepository& m_entity_repo;
  };
  
}

#endif
