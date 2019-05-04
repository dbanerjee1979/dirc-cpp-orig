#include <boost/algorithm/string.hpp>
#include "irc_server.h"
#include "irc_message.h"
#include "irc_message_commands.h"

namespace core {
  IrcServer::IrcServer(config::Network &network,
		       std::ostream &out,
		       ServerEventHandler &server_event_handler) :
    m_out(out),
    m_server_event_handler(server_event_handler),
    m_nicks(network.user_info.nicks),
    m_nick_id(0) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_WELCOME] = std::bind(&IrcServer::handle_connection_registration, this, _1);
    m_msg_handlers[RPL_MOTDSTART] = std::bind(&IrcServer::handle_motd_start, this, _1);
    m_msg_handlers[RPL_MOTD] = std::bind(&IrcServer::handle_motd, this, _1);
    m_msg_handlers[RPL_ENDOFMOTD] = std::bind(&IrcServer::handle_motd_end, this, _1);
    m_msg_handlers[ERR_NICKNAMEINUSE] = std::bind(&IrcServer::handle_nick_error, this, _1);
    m_msg_handlers[ERR_NICKCOLLISION] = std::bind(&IrcServer::handle_nick_error, this, _1);
    m_msg_handlers["NOTICE"] = std::bind(&IrcServer::handle_notice, this, _1);
    m_msg_handlers["PING"] = std::bind(&IrcServer::handle_ping, this, _1);

    if (!network.user_info.password.empty()) {
      m_out << IrcMessage("PASSWORD", { network.user_info.password }).str() << std::flush;
    }
    m_out << IrcMessage("NICK", { m_nicks[m_nick_id] }).str() << std::flush;
    m_out << IrcMessage("USER", { network.user_info.username, "8", "*" }, network.user_info.realname).str() << std::flush;
  }

  void IrcServer::quit(std::string msg) {
    m_out << IrcMessage("QUIT", {}, msg).str() << std::flush;
    m_server_event_handler.handle_shutdown();
  }

  void IrcServer::join(std::string channel) {
     m_out << IrcMessage("JOIN", { channel }).str() << std::flush;
  }

  void IrcServer::handle_message(std::string &msg_str) {
    m_server_event_handler.recieved_message(msg_str);
    IrcMessage msg(msg_str);
    auto it = m_msg_handlers.find(msg.command);
    if (it != m_msg_handlers.end()) {
      it->second(msg);
    }
    else {
      handle_message_default(msg);
    }
  }

  void IrcServer::handle_message_default(IrcMessage &msg) {
    auto it = msg.params.begin();
    auto end = msg.params.end();
    if (it != end && *it == m_nicks[m_nick_id]) {
      auto msg_str = boost::join(std::vector<std::string>(++it, end), " ");
      if (!msg.trailing.empty()) {
        if (!msg_str.empty()) {
          msg_str += " ";
        }
        msg_str += msg.trailing;
      }
      m_server_event_handler.message(msg_str);
    }
  }

  void IrcServer::handle_connection_registration(IrcMessage &msg) {
    handle_message_default(msg);
    m_server_event_handler.connected();
  }

  void IrcServer::handle_nick_error(IrcMessage &msg) {
    m_nick_id++;
    if (m_nick_id < m_nicks.size()) {
      m_out << IrcMessage("NICK", { m_nicks[m_nick_id] }).str() << std::flush;
      m_server_event_handler.error(msg.trailing);
    }
  }

  void IrcServer::handle_notice(IrcMessage &msg) {
    m_server_event_handler.notice(msg.params[0], msg.trailing);
  }

  void IrcServer::handle_motd_start(IrcMessage &msg) {
    m_motd.str("");
    m_motd << msg.trailing << std::endl;
  }

  void IrcServer::handle_motd(IrcMessage &msg) {
    m_motd << msg.trailing << std::endl;
  }

  void IrcServer::handle_motd_end(IrcMessage &msg) {
    std::string motd = m_motd.str();
    m_server_event_handler.message_of_the_day(motd);
    m_motd.str("");
  }

  void IrcServer::handle_ping(IrcMessage &msg) {
    m_out << IrcMessage("PONG", { msg.trailing }).str() << std::flush;
  }
}
