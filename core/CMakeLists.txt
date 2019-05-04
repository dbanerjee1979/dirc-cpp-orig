add_subdirectory (unit-tests)

add_library (core
  config.cpp
  irc_message.cpp
  irc_server.cpp
  irc_channel.cpp
  irc_entity_repository.cpp)
