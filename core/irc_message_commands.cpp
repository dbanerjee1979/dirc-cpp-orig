#include "irc_message_commands.h"

std::string RPL_WELCOME       = "001";
std::string RPL_MOTDSTART     = "375";
std::string RPL_MOTD          = "372";
std::string RPL_ENDOFMOTD     = "376";
std::string RPL_TOPIC         = "332";
std::string RPL_NOTOPIC       = "331";
std::string RPL_NAMREPLY      = "353";
std::string RPL_ENDOFNAMES    = "366";

std::string ERR_NICKNAMEINUSE = "433";
std::string ERR_NICKCOLLISION = "436";
