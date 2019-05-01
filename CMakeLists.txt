cmake_minimum_required (VERSION 2.6)
project (dirc)

find_package (Boost COMPONENTS system REQUIRED)
find_package (Threads REQUIRED)

add_executable (test
  main.cpp)

target_link_libraries (test
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT})
