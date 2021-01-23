//===--- main.cpp ---=========================================================//
///
/// \file
///
/// reglas basicas:
/// la ruleta tiene 37 ranuras (0 - 36).
/// pares rojos, impares negros.
/// el 0 siempre pierde.
/// color acertado: dinero x 2
/// numero acertado: dinero x 35
/// 4 data
///
/// orden de los data:
/// todos los data juegan en orden hasta que se retiren o pierdan el dinero.
///
/// ronda:
/// 1. etapa inicial: consiste en pregunta si continúa, y obtener nuevas
/// apuestas.
/// 2. giro de rueda: se calcula un nuevo número al azar.
/// 3. etapa de resultado: se muestra el resultado de cada Jugador a partir del
/// nuevo número.
/// 4. etapa final(al terminar la última ronda): muestra el resultado final de
/// cada Jugador y el saldo de la banca.
///
/// mostrar por pantalla al finalizar:
/// 1. cuanto ha ganado/perdido la banca.
/// 2. cuanto ha ganado/perdido cada Jugador.
///
///
//============================================================================//

#include "Animacion.h"
#include "Screen.h"
#include "Jugador.h"
#include "Jugadores.h"
#include "Juego.h"
#include "TerminalScreen.h"
#include <array>
#include <random>
#include <iostream>
#include <chrono>
#include <string_view>
#include <iterator>
#include <fstream>

constexpr static int creditos_iniciales{10};

auto crear_generador(std::uniform_int_distribution<int> &&dist,
                     std::random_device &&dev)
{
  return [&]() -> int { return dist(dev); };
}

Animaciones inicializar_animaciones()
{
  // Cargar imagenes
  std::ifstream stencil_file("../roulette.txt");
  auto banner{leer_imagen(stencil_file)};
  auto ruleta_ascii_1{leer_imagen(stencil_file)};
  auto ruleta_ascii_2{leer_imagen(stencil_file)};
  auto chupon{leer_imagen(stencil_file)};
  auto gallina{leer_imagen(stencil_file)};
  auto trofeo{leer_imagen(stencil_file)};
  auto splash_screen_2{leer_imagen(stencil_file)};
  stencil_file.close();

  auto imagen_1{banner + ruleta_ascii_1};
  auto imagen_2{banner + ruleta_ascii_2};

  auto animacion_espera_resultado{Animacion
    {
      .data = {std::move(imagen_1), std::move(imagen_2)},
      .interval = std::chrono::milliseconds(500),
      .times = 4
    }};
  auto animacion_trofeo{crear_animacion_default(std::move(trofeo))};
  auto animacion_se_retira{crear_animacion_default(std::move(gallina))};
  auto animacion_eliminado{crear_animacion_default(std::move(chupon))};
  auto animacion_acierta_color{
    crear_animacion_default("\nAcertaste al color.\n")};
  auto animacion_acierta_numero{
    crear_animacion_default("\nAcertaste al número.\n")};
  auto animacion_no_acierto{crear_animacion_default("\nPerdiste.\n")};
  auto animacion_splash_screen{crear_animacion_default(std::move(banner))};
  auto animacion_splash_screen_2{
    crear_animacion_default(std::move(splash_screen_2))};
  AnimacionesDeEtapaResultados animaciones_de_etapa_resultados
    {
      .acierto_color = std::move(animacion_acierta_color),
      .acierto_numero = std::move(animacion_acierta_numero),
      .acierto_ninguno = std::move(animacion_no_acierto),
      .eliminado = std::move(animacion_eliminado)
    };
  return Animaciones
    {
      .etapa_de_resultados = std::move(animaciones_de_etapa_resultados),
      .se_retira = std::move(animacion_se_retira),
      .espera_resultado = std::move(animacion_espera_resultado),
      .splash_screen_1 = std::move(animacion_splash_screen),
      .splash_screen_2 = std::move(animacion_splash_screen_2),
      .trofeo = std::move(animacion_trofeo)
    };
}

int main()
{
  Juego juego(
    TerminalScreen(inicializar_animaciones()),
    crear_generador(std::uniform_int_distribution(0, 36),
                    std::random_device{}),
    Jugadores(
      std::array<Jugador, 4>
        {
          Jugador{.id = 1, .cartera = creditos_iniciales, .apuesta = {}},
          Jugador{.id = 2, .cartera = creditos_iniciales, .apuesta = {}},
          Jugador{.id = 3, .cartera = creditos_iniciales, .apuesta = {}},
          Jugador{.id = 4, .cartera = creditos_iniciales, .apuesta = {}},
        },
      std::array<bool, 4>{true, true, true, true}
    )
  );

  try
  {
    juego.run();
  } catch (const std::runtime_error &e)
  {
    std::cerr << '\n' << e.what() << "\nCerrando...\n";
    return 0;
  } catch (...)
  {
    return 1;
  }

  return 0;
}
