#ifndef _CORE_CHAT_HANDLER_H
#define _CORE_CHAT_HANDLER_H

#include <vector>
#include "irc_channel_user.h"

namespace core {

  class ChannelUserIterator : public std::iterator<std::input_iterator_tag, IrcChannelUser> {
    std::vector<IrcChannelUser>::iterator m_it;
  public:
    ChannelUserIterator(const std::vector<IrcChannelUser>::iterator &it) : m_it(it) {}
    ChannelUserIterator(const ChannelUserIterator &it) : m_it(it.m_it) {}
    ChannelUserIterator &operator++() { ++m_it; return *this; }
    ChannelUserIterator operator++(int) { ChannelUserIterator tmp(*this); operator++(); return tmp; }
    bool operator==(const ChannelUserIterator &rhs) const { return m_it == rhs.m_it; }
    bool operator!=(const ChannelUserIterator &rhs) const { return m_it != rhs.m_it; }
    IrcChannelUser &operator*() { return *m_it; }
  };

  class ChannelEventHandler {
  public:
    ChannelEventHandler() {
    }

    virtual ~ChannelEventHandler() {
    }

    virtual void disconnected() {
    }

    virtual void topic_changed(const std::string& msg) {
    }

    virtual void channel_users(ChannelUserIterator begin, ChannelUserIterator end) {
    }

    virtual void user_quit(IrcChannelUser &_user, const std::string &msg) {
    }

    virtual void user_joined(IrcChannelUser &_user) {
    }

    virtual void user_departed(IrcChannelUser &_user, const std::string &msg) {
    }

    virtual void nick_changed(const std::string &nick_from, const std::string &nick_to) {
    }
  };
}

#endif
