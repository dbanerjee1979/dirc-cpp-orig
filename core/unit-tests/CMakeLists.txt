add_subdirectory (${GMOCK_DIR} ${CMAKE_BINARY_DIR}/gmock)

include_directories (
  ${GMOCK_DIR}/googletest/include
  ${GMOCK_DIR}/googlemock/include)

add_executable (core-test
  irc_message_test.cpp
  irc_server_test.cpp
  irc_entity_repository_test.cpp)

target_link_libraries (core-test
  core
  gmock_main)

add_test (core-test core-test)
