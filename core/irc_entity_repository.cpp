#include "irc_message_commands.h"
#include "irc_entity_repository.h"

namespace core {

  IrcEntityRepository::IrcEntityRepository() {
    m_entity_idx[RPL_TOPIC]      = 1;
    m_entity_idx[RPL_NOTOPIC]    = 1;
    m_entity_idx[RPL_NAMREPLY]   = 2;
    m_entity_idx[RPL_ENDOFNAMES] = 1;
    m_entity_idx["JOIN"]         = 0;
  }

  void IrcEntityRepository::create_channel(const std::string &channel,
                                           ChannelEventHandler *channel_handler) {
    auto ch = new IrcChannel(channel, channel_handler, *this);
    m_channels[channel] = std::unique_ptr<IrcChannel>(ch);
  }

  boost::optional<IrcChannel&> IrcEntityRepository::find_channel(const IrcMessage &msg) {
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
    auto user = new IrcUser(nickname, username, realname);
    m_users[nickname] = std::unique_ptr<IrcUser>(user);
  }

  void IrcEntityRepository::remove_user(const std::string &nickname) {
    m_users.erase(nickname);
  }

  boost::optional<IrcUser&> IrcEntityRepository::find_user(const std::string &nickname) {
    auto it = m_users.find(nickname);
    if (it != m_users.end()) {
      return *(it->second);
    }
    return boost::none;
  }

}
