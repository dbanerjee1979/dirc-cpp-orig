add_executable (dirc-text
  main.cpp)

target_link_libraries (dirc-text
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT})
