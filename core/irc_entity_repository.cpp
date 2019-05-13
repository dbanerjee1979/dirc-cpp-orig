#include "irc_message_commands.h"
#include "irc_entity_repository.h"
#include "user_event_handler.h"

namespace core {

  IrcEntityRepository::IrcEntityRepository() {
    m_entity_idx[RPL_TOPIC]      = 1;
    m_entity_idx[RPL_NOTOPIC]    = 1;
    m_entity_idx[RPL_NAMREPLY]   = 2;
    m_entity_idx[RPL_ENDOFNAMES] = 1;
    m_entity_idx["JOIN"]         = 0;
    m_entity_idx["PART"]         = 0;
  }

  void IrcEntityRepository::create_channel(const std::string &channel, std::ostream &out) {
    struct DisconnectHandler : public ChannelEventHandler {
      IrcEntityRepository &m_entity_repo;
      std::string m_channel;
      DisconnectHandler(IrcEntityRepository &entity_repo, std::string channel) :
        m_entity_repo(entity_repo), m_channel(channel) {
      }
      void disconnected() {
        m_entity_repo.m_channels.erase(m_channel);
      }
    };
    auto ch = new IrcChannel(channel, out, *this);
    ch->add_event_handler(std::shared_ptr<DisconnectHandler>(new DisconnectHandler(*this, channel)));
    m_channels[channel] = std::unique_ptr<IrcChannel>(ch);
  }

  boost::optional<IrcChannel &> IrcEntityRepository::find_channel(const IrcMessage &msg) {
    auto it = m_entity_idx.find(msg.command);
    int idx;
    if (it != m_entity_idx.end() && (msg.params.size() > (idx = it->second))) {
      const std::string &channel = msg.params[idx];
      auto it = m_channels.find(channel);
      if (it != m_channels.end()) {
        return *(it->second);
      }
    }
    return boost::none;
  }

  void IrcEntityRepository::foreach_channels(std::function<void(IrcChannel &)> handler) {
    for (auto it = m_channels.begin(); it != m_channels.end(); it++) {
      handler(*it->second);
    }
  }

  void IrcEntityRepository::create_user(const std::string &nickname,
                                        const std::string &username,
                                        const std::string &realname) {
    struct NickChangeHandler : public UserEventHandler {
      IrcEntityRepository &m_er;
      std::string m_nick;
      NickChangeHandler(IrcEntityRepository &er, const std::string &nick) : m_er(er), m_nick(nick) {
      }
      void nick_changed(const std::string &nick_from, const std::string &nick_to) {
        auto it = m_er.m_users.find(nick_from);
        if (it != m_er.m_users.end()) {
          m_er.m_users[nick_to] = std::move(it->second);
          m_er.m_users.erase(it);
        }
      }
      void quit(const std::string &) {
        m_er.m_users.erase(m_nick);
      }
    };
    auto user = new IrcUser(nickname, username, realname);
    user->add_event_handler(std::shared_ptr<NickChangeHandler>(new NickChangeHandler(*this, nickname)));
    m_users[nickname] = std::unique_ptr<IrcUser>(user);
  }

  void IrcEntityRepository::remove_user(const std::string &nickname) {
    m_users.erase(nickname);
  }

  boost::optional<IrcUser &> IrcEntityRepository::find_user(const IrcMessage &msg) {
    return msg.nick.empty() ? boost::none : find_user(msg.nick);
  }

  boost::optional<IrcUser &> IrcEntityRepository::find_user(const std::string &nickname) {
    auto it = m_users.find(nickname);
    if (it != m_users.end()) {
      return *(it->second);
    }
    return boost::none;
  }

}
