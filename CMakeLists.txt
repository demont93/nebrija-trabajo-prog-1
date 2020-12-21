project(nebrija-trabajo-prog-1)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
add_executable(ruleta main.cpp)
add_executable(ruleta2 main2.cpp)

add_custom_command(
        TARGET ruleta
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/roulette.txt ${CMAKE_BINARY_DIR}
)
