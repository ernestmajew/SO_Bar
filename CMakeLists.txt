cmake_minimum_required(VERSION 3.24)
project(SO_Bar)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-lncurses -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(SO_Bar main.cpp Bar.cpp Bar.h)

target_link_libraries(SO_Bar ${CURSES_LIBRARIES})
