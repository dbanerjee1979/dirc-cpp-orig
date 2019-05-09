#include "irc_message.h"

#include <sstream>
#include "irc_message_parser.h"

namespace core {

  IrcMessage::IrcMessage(const std::string &msg) {
    IrcMessageParser p(msg);

    if (p.is_char(':')) {
      p.skip();
      if (p.is_servername()) {
	p.token(servername);
      }
      else if (p.is_nickname()) {
	p.token(nick);
	if (p.is_user()) {
	  p.token(user);
	}
	if (p.is_host()) {
	  p.token(host);
	}
      }
      if (p.is_char(' ')) {
	p.skip();
      }
    }

    if (p.is_command()) {
      p.token(command);
    }

    if (!command.empty()) {
      while (p.is_param()) {
	params.push_back("");
	std::string &param = params.back();
	p.token(param);
      }

      if (p.is_trailing()) {
	p.token(trailing);
      }
    }
  }

  IrcMessage::IrcMessage(const std::string &_command, const std::vector<std::string> &_params) :
    command(_command),
    params(_params) {
  }

  IrcMessage::IrcMessage(const std::string &_command, const std::vector<std::string> &_params, const std::string &_trailing) :
    command(_command),
    params(_params),
    trailing(_trailing) {
  }

  std::string IrcMessage::str() {
    std::stringstream ss;
    if (!servername.empty()) {
      ss << ":" << servername << " ";
    }
    if (!nick.empty()) {
      ss << ":" << nick;
      if (!user.empty()) {
        ss << "!" << user;
      }
      if (!host.empty()) {
        ss << "@" << host;
      }
      ss << " ";
    }
    ss << command;
    for (auto it = params.begin(); it != params.end(); ++it) {
      ss << " " << *it;
    }
    if (!trailing.empty()) {
      ss << " :" << trailing;
    }
    ss << "\r\n";
    return ss.str();
  }
}

