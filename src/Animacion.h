#pragma once

#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

struct Animacion
{
  std::vector<std::string> data;
  std::chrono::milliseconds interval;
  int times;
};

void animar(const Animacion &animacion)
{
  for (int i{}, j{}; i < animacion.times; ++i, ++j)
  {
    if (j == animacion.data.size()) j = 0;
    auto begin{std::chrono::system_clock::now()};
    std::cout << animacion.data[j] << std::flush;
    std::this_thread::sleep_for(
      animacion.interval - (std::chrono::system_clock::now() - begin));
  }
}

void mostrar_imagen(
  const std::string &str,
  const std::chrono::duration<int> &duration = std::chrono::seconds(0))
{
  std::cout << str << std::flush;
  std::this_thread::sleep_for(duration);
}

Animacion crear_animacion_default(std::string str)
{
  return
    {
      .data = std::vector<std::string>{std::move(str)},
      .interval = std::chrono::milliseconds(0),
      .times = 1
    };
}
