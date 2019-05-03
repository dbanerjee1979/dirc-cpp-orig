add_executable (dirc-text
  main.cpp
  text_app.cpp
  text_server_event_handler.cpp)

target_link_libraries (dirc-text
  core
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT})
