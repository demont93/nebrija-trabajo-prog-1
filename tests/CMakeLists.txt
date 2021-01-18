find_library(pthread NAMES threads pthread REQUIRED)

enable_testing()
find_package(GTest CONFIG REQUIRED)
add_executable(TestBasics TestBasics.cpp)
target_link_libraries(TestBasics gtest gtest_main pthread)
add_test(TestBasics TestBasics)
