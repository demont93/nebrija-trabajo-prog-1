project(nebrija-trabajo-prog-1)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

find_library(ncurses ncurses)
add_subdirectory(src)

