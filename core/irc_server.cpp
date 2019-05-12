#include <boost/algorithm/string.hpp>
#include "irc_server.h"
#include "irc_message.h"
#include "irc_message_commands.h"

namespace core {
  IrcServer::IrcServer(config::Network &network,
		       std::ostream &out,
		       ServerEventHandler &event_handler,
                       IrcEntityRepository &entity_repo) :
    m_out(out),
    m_event_handler(event_handler),
    m_user_info(network.user_info),
    m_nick_id(0),
    m_entity_repo(entity_repo) {

    using std::placeholders::_1;
    m_msg_handlers[RPL_WELCOME] = std::bind(&IrcServer::handle_connection_registration, this, _1);
    m_msg_handlers[RPL_MOTDSTART] = std::bind(&IrcServer::handle_motd_start, this, _1);
    m_msg_handlers[RPL_MOTD] = std::bind(&IrcServer::handle_motd, this, _1);
    m_msg_handlers[RPL_ENDOFMOTD] = std::bind(&IrcServer::handle_motd_end, this, _1);
    m_msg_handlers[ERR_NICKNAMEINUSE] = std::bind(&IrcServer::handle_nick_error, this, _1);
    m_msg_handlers[ERR_NICKCOLLISION] = std::bind(&IrcServer::handle_nick_error, this, _1);
    m_msg_handlers["NOTICE"] = std::bind(&IrcServer::handle_notice, this, _1);
    m_msg_handlers["PING"] = std::bind(&IrcServer::handle_ping, this, _1);
    m_msg_handlers["JOIN"] = std::bind(&IrcServer::handle_join, this, _1);
    m_msg_handlers["QUIT"] = std::bind(&IrcServer::handle_quit, this, _1);
    m_msg_handlers["NICK"] = std::bind(&IrcServer::handle_nick, this, _1);

    if (!network.user_info.password.empty()) {
      m_out << IrcMessage("PASSWORD", { network.user_info.password }).str() << std::flush;
    }
    m_out << IrcMessage("NICK", { nick() }).str() << std::flush;
    m_out << IrcMessage("USER", { network.user_info.username, "8", "*" }, network.user_info.realname).str() << std::flush;
  }

  void IrcServer::quit(const std::string &msg) {
    m_out << IrcMessage("QUIT", {}, msg).str() << std::flush;
    m_event_handler.handle_shutdown();
  }

  void IrcServer::join(const std::string &channel) {
     m_out << IrcMessage("JOIN", { channel }).str() << std::flush;
  }

  const std::string &IrcServer::nick() {
    return m_user_info.nicks[m_nick_id];
  }

  void IrcServer::handle_message(const std::string &msg_str) {
    m_event_handler.recieved_message(msg_str);
    IrcMessage msg(msg_str);
    auto channel = m_entity_repo.find_channel(msg);
    if (channel) {
      channel->handle_message(msg);
    }
    else {
      auto it = m_msg_handlers.find(msg.command);
      if (it != m_msg_handlers.end()) {
        it->second(msg);
      }
      else {
        handle_message_default(msg);
      }
    }
  }

  void IrcServer::handle_message_default(const IrcMessage &msg) {
    auto it = msg.params.begin();
    auto end = msg.params.end();
    if (it != end && *it == m_user_info.nicks[m_nick_id]) {
      auto msg_str = boost::join(std::vector<std::string>(++it, end), " ");
      if (!msg.trailing.empty()) {
        if (!msg_str.empty()) {
          msg_str += " ";
        }
        msg_str += msg.trailing;
      }
      m_event_handler.message(msg_str);
    }
  }

  void IrcServer::handle_connection_registration(const IrcMessage &msg) {
    handle_message_default(msg);
    m_entity_repo.create_user(nick(), m_user_info.username, m_user_info.realname);
    m_event_handler.connected();
  }

  void IrcServer::handle_nick_error(const IrcMessage &msg) {
    m_nick_id++;
    if (m_nick_id < m_user_info.nicks.size()) {
      m_out << IrcMessage("NICK", { nick() }).str() << std::flush;
      m_event_handler.error(msg.trailing);
    }
  }

  void IrcServer::handle_notice(const IrcMessage &msg) {
    if (msg.params.size() > 0) {
      m_event_handler.notice(msg.params[0], msg.trailing);
    }
  }

  void IrcServer::handle_motd_start(const IrcMessage &msg) {
    m_motd.str("");
    m_motd << msg.trailing << std::endl;
  }

  void IrcServer::handle_motd(const IrcMessage &msg) {
    m_motd << msg.trailing << std::endl;
  }

  void IrcServer::handle_motd_end(const IrcMessage &msg) {
    std::string motd = m_motd.str();
    m_event_handler.message_of_the_day(motd);
    m_motd.str("");
  }

  void IrcServer::handle_ping(const IrcMessage &msg) {
    m_out << IrcMessage("PONG", { msg.trailing }).str() << std::flush;
  }

  void IrcServer::handle_join(const IrcMessage &msg) {
    const std::string &channel = msg.params[0];
    if (nick() == msg.nick) {
      std::shared_ptr<ChannelEventHandler> ch(m_event_handler.create_channel_event_handler(channel));
      if (ch) {
        m_entity_repo.create_channel(channel, m_out, ch);
      }
    }
  }

  void IrcServer::handle_quit(const IrcMessage &msg) {
    boost::optional<IrcUser> user;
    if (!msg.nick.empty() && (user = m_entity_repo.find_user(msg.nick))) {
      m_event_handler.user_quit(*user, msg.trailing);
      m_entity_repo.foreach_channels([msg] (IrcChannel& ch) { ch.handle_message(msg); });
      m_entity_repo.remove_user(msg.nick);
    }
  }

  void IrcServer::handle_nick(const IrcMessage &msg) {
    boost::optional<IrcUser> user;
    auto nick_from = msg.nick;
    if (msg.params.size() > 0 && !msg.nick.empty() && (user = m_entity_repo.find_user(nick_from))) {
      auto nick_to = msg.params[0];
      m_entity_repo.change_nick(nick_from, nick_to);
      m_event_handler.nick_changed(nick_from, nick_to);
      m_entity_repo.foreach_channels([msg] (IrcChannel &ch) { ch.handle_message(msg); });
    }
  }
}
