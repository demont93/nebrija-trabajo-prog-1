#include "Animacion.h"

Animacion crear_animacion_default(std::string str)
{
  return
    {
      .data = std::vector<std::string>{std::move(str)},
      .interval = std::chrono::milliseconds(0),
      .times = 1
    };
}
