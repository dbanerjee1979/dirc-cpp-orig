add_executable (dirc-text
  main.cpp
  text_app.cpp)

target_link_libraries (dirc-text
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT})
