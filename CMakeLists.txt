project(nebrija-trabajo-prog-1)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(tests)
ENDIF ()

