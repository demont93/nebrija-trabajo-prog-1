#pragma once

#include "Screen.h"
#include "Jugadores.h"
#include <thread>
#include <numeric>
#include <algorithm>

class TerminalScreen
{
 public:
  explicit TerminalScreen(Animaciones animaciones);

  TerminalScreen(TerminalScreen &&rhs) noexcept = default;

  TerminalScreen(const TerminalScreen &rhs) = default;

  TerminalScreen &operator=(TerminalScreen &&rhs) noexcept = default;

  TerminalScreen &operator=(const TerminalScreen &rhs) = default;

  static void mensaje(const std::string &s);

  static void status_jugador(const JugadorProxy &jugador);

  static void status_final_jugador(const JugadorProxy &jugador);

  static void mensaje_pedir_apuesta(const JugadorProxy &jugador);

  static void mensaje_eleccion_ruleta(const JugadorProxy &jugador);

  static void resultado_final_banco(int dinero_banca);

  static void tabla_de_resultados(const std::vector<Resultado> &filas);

  void resultado_ultimo_giro(int giro);

  template<typename R>
  requires std::ranges::input_range<R> &&
           std::same_as<std::ranges::range_value_t<R>, JugadorProxy>
  void resultado_final(R &&jugadores, int banca);

  void bienvenida() const;

  void se_retira() const;

 private:
  Animaciones animaciones;

  static void animar(const Animacion &animacion);

  static void crear_header(std::ostringstream &out,
                           const std::array<int, 7> &width,
                           const std::array<const char *, 7> &row);

  static void crear_fila(std::ostringstream &out,
                         const Resultado &resultado,
                         const std::array<int, 7> &width);

  static void clear();

  static void newline();
};

template<typename R>
requires std::ranges::input_range<R> &&
         std::same_as<std::ranges::range_value_t<R>, JugadorProxy>
void TerminalScreen::resultado_final(R &&jugadores, int banca)
{
  newline();
  resultado_final_banco(banca);

  std::ranges::for_each(jugadores, [](const JugadorProxy &jugador)
  {
    newline();
    status_jugador(jugador);
    status_final_jugador(jugador);
  });

  auto max_creditos{std::max_element(
    jugadores.begin(), jugadores.end(),
    [](const JugadorProxy &prev, const JugadorProxy &next)
    {
      return prev.get_creditos() < next.get_creditos();
    })};

  auto en_posicion_1 = std::bind(
    std::ranges::count_if, jugadores, [&](const JugadorProxy &jugador)
    {
      return jugador.get_creditos() == (*max_creditos).get_creditos();
    });

  if ((*max_creditos).get_creditos() > 0 && en_posicion_1() == 1)
  {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    newline();
    animar(animaciones.trofeo);
    std::ostringstream ss{};
    ss << "      Jugador " << (*max_creditos).get_id()
       << " tienes el primer lugar.";
    mensaje(ss.str());
  }
}


