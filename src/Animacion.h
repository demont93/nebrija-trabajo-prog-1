#pragma once

#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

void animacion(const std::vector<std::string> &images,
               const std::chrono::milliseconds &interval,
               int times)
{
  for (int i{}, j{}; i < times; ++i, ++j)
  {
    if (j == images.size()) j == 0;
    auto begin{std::chrono::system_clock::now()};
    std::cout << images[j] << std::flush;
    std::this_thread::sleep_for(
      interval - (std::chrono::system_clock::now() - begin));
  }
}

void mostrar_imagen(
  const std::string &str,
  const std::chrono::duration<int> &duration = std::chrono::seconds(0))
{
  std::cout << str << std::flush;
  std::this_thread::sleep_for(duration);
}
