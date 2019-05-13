#include "irc_message.h"

#include <sstream>
#include <boost/algorithm/string.hpp>
#include "irc_message_parser.h"

namespace core {

  IrcMessage::IrcMessage(const std::string &msg) {
    IrcMessageParser p(msg);

    if (p.is_char(':')) {
      p.skip();
      if (p.is_servername()) {
	p.token(m_servername);
      }
      else if (p.is_nickname()) {
	p.token(m_nick);
	if (p.is_user()) {
	  p.token(m_user);
	}
	if (p.is_host()) {
	  p.token(m_host);
	}
      }
      if (p.is_char(' ')) {
	p.skip();
      }
    }

    if (p.is_command()) {
      p.token(m_command);
    }

    if (!m_command.empty()) {
      while (p.is_param()) {
	m_params.push_back("");
	std::string &param = m_params.back();
	p.token(param);
      }

      if (p.is_trailing()) {
	p.token(m_trailing);
      }
    }
  }

  IrcMessage::IrcMessage(const std::string &command, const std::vector<std::string> &params) :
    m_command(command),
    m_params(params) {
  }

  IrcMessage::IrcMessage(const std::string &command, const std::vector<std::string> &params, const std::string &trailing) :
    m_command(command),
    m_params(params),
    m_trailing(trailing) {
  }

  std::string IrcMessage::str() {
    std::stringstream ss;
    if (!m_servername.empty()) {
      ss << ":" << m_servername << " ";
    }
    if (!m_nick.empty()) {
      ss << ":" << m_nick;
      if (!m_user.empty()) {
        ss << "!" << m_user;
      }
      if (!m_host.empty()) {
        ss << "@" << m_host;
      }
      ss << " ";
    }
    ss << m_command;
    for (auto it = m_params.begin(); it != m_params.end(); ++it) {
      ss << " " << *it;
    }
    if (!m_trailing.empty()) {
      ss << " :" << m_trailing;
    }
    ss << "\r\n";
    return ss.str();
  }

  const std::string &IrcMessage::servername() const {
    return m_servername;
  }
  
  const std::string &IrcMessage::nick() const {
    return m_nick;
  }
  
  const std::string &IrcMessage::user() const {
    return m_user;
  }
  
  const std::string &IrcMessage::host() const {
    return m_host;
  }
  
  const std::string &IrcMessage::command() const {
    return m_command;
  }

  const std::string &IrcMessage::trailing() const {
    return m_trailing;
  }

  const int IrcMessage::param_count() {
    return m_params.size() + (m_trailing.empty() ? 0 : 1);
  }

  const std::string EMPTY_PARAM = "";
  
  const std::string &IrcMessage::param(int n) const {
    return n < m_params.size() ? m_params[n] :
      n == m_params.size() ? m_trailing :
      EMPTY_PARAM;
  }

  std::string IrcMessage::param_str() const {
    auto it = m_params.begin();
    auto end = m_params.end();
    std::string msg_str = it != end ? boost::join(std::vector<std::string>(++it, end), " ") : "";
    if (!m_trailing.empty()) {
      if (!msg_str.empty()) {
        msg_str += " ";
      }
      msg_str += m_trailing;
    }
    return msg_str;
  }
  
}
