cmake_minimum_required (VERSION 3.0)
project (dirc)

enable_testing ()

find_package (Boost COMPONENTS system REQUIRED)
find_package (Threads REQUIRED)

set (GMOCK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third-party/googletest")

add_subdirectory (core)
add_subdirectory (text)
