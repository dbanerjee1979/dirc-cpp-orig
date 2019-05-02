#include "irc_message.h"

#include <sstream>

namespace core {

  struct Parser {
    std::stringstream ss;
    std::string::const_iterator it;
    std::string::const_iterator end;

    Parser(const std::string& msg) :
      it(msg.begin()),
      end(msg.end()) {
    }

    std::string str() {
      std::string ret = ss.str();
      ss.str("");
      return ret;
    }

    void skip() {
      it++;
    }

    bool is_char(char ch) {
      return it != end && *it == ch;
    }

    bool is_char(char cbegin, char cend) {
      return it != end && *it >= cbegin && *it <= cend;
    }

    bool is_letter() {
      return is_char('A', 'Z') || is_char('a', 'z');
    }

    bool is_digit() {
      return is_char('0', '9');
    }

    bool is_special() {
      return is_char('[') || is_char(']') || is_char('\\')
	|| is_char('`') || is_char('_') || is_char('^')
	|| is_char('{') || is_char('|') || is_char('}');
    }

    bool is_nospcrlfcl() {
      return it == end || (*it != ' ' && *it != '\r' && *it != '\n' && *it != ':');
    }

    bool is_end() {
      return it == end || *it == ' ' || *it == '\r';
    }

    bool is_servername() {
      bool dot = false;
      auto start = it;
      ss.str("");
      
      while (is_char('A', 'Z') || is_char('a', 'z') || is_char('0', '9') || is_char('-') || is_char('.')) {
	if (is_char('.')) {
	  dot = true;
	}
	ss << *it++;
      }
      
      if (!dot) {
	it = start;
      }
      return dot;
    }

    bool is_nickname() {
      bool nick = false;
      auto start = it;
      ss.str("");
      
      if (!is_letter() && !is_special()) {
	return false;
      }
      do {
	ss << *it++;
	nick = true;
      }
      while (is_letter() || is_digit() || is_special() || is_char('-'));

      if (!nick) {
	it = start;
      }
      return nick;
    }

    bool is_user() {
      if (!is_char('!')) {
	return false;
      }
      it++;
      
      while (!is_char('@')) {
	ss << *it++;
      }
      return is_char('@');
    }

    bool is_host() {
      if (!is_char('@')) {
	return false;
      }
      it++;
      
      while (!is_char(' ')) {
	ss << *it++;
      }
      return is_char(' ');
    }

    bool is_command() {
      while (is_letter()) {
	ss << *it++;
      }
      return is_end();
    }

    bool is_param() {
      if (!is_char(' ')) {
        return false;
      }
      it++;

      if (!is_nospcrlfcl()) {
	return false;
      }

      do {
	ss << *it++;
      }
      while (is_char(':') || is_nospcrlfcl());
      return true;
    }

    bool is_trailing() {
      // Leading space check unnecessary, because is_param already checked
      if (!is_char(':')) {
	return false;
      }
      it++;

      while (is_char(':') || is_char(' ') || is_nospcrlfcl()) {
	ss << *it++;
      }
      return true;
    }
  };

  IrcMessage::IrcMessage(const std::string &msg) {
    Parser p(msg);

    if (p.is_char(':')) {
      p.skip();
      if (p.is_servername()) {
	servername = p.str();
      }
      else if (p.is_nickname()) {
	nick = p.str();
	if (p.is_user()) {
	  user = p.str();
	}
	if (p.is_host()) {
	  host = p.str();
	}
      }
      if (p.is_char(' ')) {
	p.skip();
      }
    }

    if (p.is_command()) {
      command = p.str();
    }
    
    while (p.is_param()) {
      params.push_back(p.str());
    }

    if (p.is_trailing()) {
      trailing = p.str();
    }
  }
}

