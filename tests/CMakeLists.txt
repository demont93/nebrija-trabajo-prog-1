enable_testing()
find_library(pthread NAMES threads pthread REQUIRED)
find_package(GTest 1 CONFIG REQUIRED)

add_executable(test-dummy TestDummy.cpp)
target_link_libraries(test-dummy gtest gtest_main pthread)

### Tests ######################################################################

add_test(dummy test-dummy --gtest_filter=Dummy.dummy)

