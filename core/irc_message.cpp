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

    bool is_char(char ch) {
      return it != end && *it == ch;
    }

    bool is_letter() {
      return it != end && (*it >= 'A' && *it <= 'Z' || *it >= 'a' && *it <= 'z');
    }

    bool is_nospcrlfcl() {
      return it == end || (*it != ' ' && *it != '\r' && *it != '\n' && *it != ':');
    }

    bool is_end() {
      return it == end || *it == ' ' || *it == '\r';
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
      ss << *it++;
      
      while (is_char(':') || is_nospcrlfcl()) {
	ss << *it++;
      }
      return true;
    }

    bool is_trailing() {
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

