#include <gtest/gtest.h>
#include "../src/Animacion.h"
#include <chrono>

TEST(TestBasics, test_basics)
{
  using namespace std::chrono_literals;
  std::vector<std::string> vec{"=====", "||||||"};
  animacion(vec, 1s, 4);
  ASSERT_EQ(2, 2);
}