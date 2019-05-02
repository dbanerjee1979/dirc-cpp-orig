#include "irc_message.h"

#include <sstream>

namespace core {

  struct IrcMessageParser {
    // Current position
    std::string::const_iterator it;
    // Token start
    std::string::const_iterator st;
    // End position
    std::string::const_iterator end;

    IrcMessageParser(const std::string& msg) :
      it(msg.begin()),
      st(it),
      end(msg.end()) {
    }

    //
    // Predicates
    //

    std::string str() {
      return std::string(st, it);
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

    //
    // Tokens
    //

    bool is_servername() {
      st = it;
      bool dot = false;
      
      while (is_char('A', 'Z') || is_char('a', 'z') || is_char('0', '9') || is_char('-') || is_char('.')) {
	if (is_char('.')) {
	  dot = true;
	}
	it++;
      }

      // A servername token must have a dot, not only a dot and can't start with a dot
      if (!dot || it - st == 1 || *st == '.') {
	it = st;
	return false;
      }
      return true;
    }

    bool is_nickname() {
      st = it;
      
      if (!is_letter() && !is_special()) {
	return false;
      }
      
      do {
	it++;
      }
      while (is_letter() || is_digit() || is_special() || is_char('-'));

      return it != st;
    }

    bool is_user() {
      st = it;
      
      if (!is_char('!')) {
	return false;
      }
      it++;
      
      while (it != end && !is_char('@')) {
	it++;
      }

      // Username must be followed by start of hostname
      // and must have nonzero characters after !
      if (is_char('@') && it - st > 1) {
	st++;
	return true;
      }
      it = st;
      return false;
    }

    bool is_host() {
      st = it;
      
      if (!is_char('@')) {
	return false;
      }
      it++;
      
      while (it != end && !is_char(' ')) {
	it++;
      }

      // Hostname must be followed by space
      // and must have nonzero characters after @
      if (is_char(' ') && it - st > 1) {
	st++;
	return true;
      }
      it = st;
      return false;
    }

    bool is_command() {
      st = it;

      // Try alphabetic command
      while (is_letter()) {
	it++;
      }

      // If not alpabetic, try numeric
      if (st == it) {
	while (is_digit()) {
	  it++;
	}
	// Numeric commands are three in length
	if (it - st != 3) {
	  st = it;
	}
      }
      
      return st != it;
    }

    bool is_param() {
      st = it;
      
      if (!is_char(' ')) {
        return false;
      }
      it++;

      if (!is_nospcrlfcl()) {
	it = st;
	return false;
      }

      st = it;
      do {
	it++;
      }
      while (is_char(':') || is_nospcrlfcl());
      return true;
    }

    bool is_trailing() {
      st = it;

      if (!is_char(' ')) {
        return false;
      }
      it++;

      if (!is_char(':')) {
	it = st;
	return false;
      }
      it++;

      st = it;
      while (is_char(':') || is_char(' ') || is_nospcrlfcl()) {
	it++;
      }
      return true;
    }
  };

  IrcMessage::IrcMessage(const std::string &msg) {
    IrcMessageParser p(msg);

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

    if (!command.empty()) {
      while (p.is_param()) {
	params.push_back(p.str());
      }

      if (p.is_trailing()) {
	trailing = p.str();
      }
    }
  }
}

