project(nebrija-trabajo-prog-1)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
add_executable(ruleta main.cpp)
find_library(ncurses ncurses)
target_link_libraries(ruleta ncurses)

add_custom_command(
        TARGET ruleta
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/roulette.txt ${CMAKE_BINARY_DIR}
)
