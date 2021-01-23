#pragma once

#include "Screen.h"
#include "Jugadores.h"
#include <algorithm>

template<Screen S, typename G>
requires std::same_as<std::invoke_result_t<G>, int>
class Juego
{
 public:
  Juego(S screen, G generator, const Jugadores &jugadores);

  void run();

 private:
  S screen;
  G generator;
  Jugadores jugadores;

  // Gira la rueda, produciendo un numero y mostrandolo.
  int giro_de_rueda(auto &generador_random_ruleta);
};

template<Screen S, typename G>
requires std::same_as<std::invoke_result_t<G>, int>
Juego<S, G>::Juego(S screen, G generator, const Jugadores &jugadores)
  : screen(std::move(screen)), generator(std::move(generator)),
    jugadores(jugadores) {}

template<Screen S, typename G>
requires std::same_as<std::invoke_result_t<G>, int>
void Juego<S, G>::run()
{
  screen.bienvenida();

  // Mientras existan jugadores activos;
  for (auto activos{jugadores.activos()};
       !activos.empty();
       activos = jugadores.activos())
  {
    for (auto activo : activos)
      activo.etapa_inicial(screen);

    // Si quedan _activos, girar rueda y manejar resultados.
    activos = jugadores.activos();
    if (activos.empty()) break;

    auto resultado_ruleta{giro_de_rueda(generator)};
    screen.resultado_ultimo_giro(resultado_ruleta);

    std::vector<Resultado> resultados{};
    resultados.reserve(activos.size());
    std::ranges::transform(
      activos,
      std::back_inserter(resultados),
      [=](auto &resultado)
      {
        return resultado.etapa_de_resultados(resultado_ruleta);
      });

    S::tabla_de_resultados(std::move(resultados));
  }

  // Resultado final para cada Jugador y el saldo final del banco.
  jugadores.etapa_final(screen);
}

template<Screen S, typename G>
requires std::same_as<std::invoke_result_t<G>, int>
int Juego<S, G>::giro_de_rueda(auto &generador_random_ruleta)
{
  auto resultado_ruleta{generador_random_ruleta()};
  return resultado_ruleta;
}
