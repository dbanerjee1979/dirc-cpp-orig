#ifndef _CORE_IRC_MESSAGE_PARSER_H
#define _CORE_IRC_MESSAGE_PARSER_H

#include <string>

namespace core {

  struct IrcMessageParser {
    // Current position
    std::string::const_iterator it;
    // Token start
    std::string::const_iterator st;
    // End position
    std::string::const_iterator end;

    IrcMessageParser(const std::string& msg);
    void token(std::string &recv);
    void skip();

    //
    // Predicates
    //

    bool is_char(char ch);
    bool is_char(char cbegin, char cend);
    bool is_letter();
    bool is_digit();
    bool is_special();
    bool is_nospcrlfcl();

    //
    // Tokens
    //

    bool is_servername();
    bool is_nickname();
    bool is_user();
    bool is_host();
    bool is_command();
    bool is_param();
    bool is_trailing();
  };
  
}

#endif
