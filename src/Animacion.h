#pragma once

#include <vector>
#include <string>
#include <chrono>

struct Animacion
{
  std::vector<std::string> data;
  std::chrono::milliseconds interval;
  int times;
};

Animacion crear_animacion_default(std::string str);
