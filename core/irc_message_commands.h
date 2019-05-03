#ifndef _CORE_IRC_MESSAGE_COMMANDS_H
#define _CORE_IRC_MESSAGE_COMMANDS_H

#include <string>

std::string RPL_WELCOME       = "001";
std::string RPL_MOTDSTART     = "375";
std::string RPL_MOTD          = "372";
std::string RPL_ENDOFMOTD     = "376";

std::string ERR_NICKNAMEINUSE = "433";
std::string ERR_NICKCOLLISION = "436";

#endif
