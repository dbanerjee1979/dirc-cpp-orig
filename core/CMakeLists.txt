add_subdirectory (unit-tests)

add_library (core
  irc_server.cpp
  irc_message.cpp
  config.cpp)
