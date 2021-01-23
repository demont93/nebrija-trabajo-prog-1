#include "TerminalScreen.h"
#include "Screen.h"
#include "Animacion.h"
#include "Jugadores.h"
#include "IO.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread>

void TerminalScreen::mensaje(const std::string &s)
{
  std::cout << s << '\n';
}

void TerminalScreen::animar(const Animacion &animacion)
{
  for (int i{}, j{}; i < animacion.times; ++i, ++j)
  {
    if (j == animacion.data.size()) j = 0;
    auto begin{std::chrono::system_clock::now()};
    clear();
    std::cout << animacion.data[j] << std::flush;
    std::this_thread::sleep_for(
      animacion.interval - (std::chrono::system_clock::now() - begin));
  }
}

void TerminalScreen::resultado_final_banco(int dinero_banca)
{
  std::ostringstream out{};
  out << "Banco quedó con un saldo de " << format_dinero(dinero_banca) << '\n';
  mensaje(out.str());
}

void TerminalScreen::status_jugador(const JugadorProxy &jugador)
{
  std::ostringstream out{};
  out << "Jugador #" << jugador.get_id() << "\nCréditos: "
      << format_dinero(jugador.get_creditos());
  mensaje(out.str());
}

void TerminalScreen::mensaje_pedir_apuesta(const JugadorProxy &jugador)
{
  std::ostringstream out{};
  out << "Disponible: " << jugador.get_creditos();
  out << "\nIntroduce tu Apuesta:";
  mensaje(out.str());
}

void TerminalScreen::mensaje_eleccion_ruleta(const JugadorProxy &jugador)
{
  std::ostringstream out{};
  out << "Introduce el número de la ruleta que vas a elegir.\nRecuerda que los "
         "numeros pares son rojos y los impares negros.";
  mensaje(out.str());
}

void TerminalScreen::status_final_jugador(const JugadorProxy &jugador)
{
  std::ostringstream out{};
  auto creditos{jugador.get_creditos()};
  if (creditos)
  {
    if (creditos > 10)
      out << "Lograste salir victorioso. ¡Felicitactiones!";
    else if (creditos == 10)
      out << "Aqui no ha pasado nada.";
    else
      out << "A veces es mejor saber cuando retirarse. Conservaste algo de "
             "tu inversión inicial.";
  } else
  {
    out << "Lo perdiste todo.";
  }
  mensaje(out.str());
}

void TerminalScreen::bienvenida() const
{
  animar(animaciones.splash_screen_2);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  animar(animaciones.splash_screen_1);
  constexpr const char *out{
    "Bienvenido al juego, cada quien comienza con 10 euros.\n"
    "Cada ronda puedes jugar o retirarte. Si pierdes todo, estás fuera."};
  mensaje(out);
}

void TerminalScreen::resultado_ultimo_giro(int resultado)
{
  animar(animaciones.espera_resultado);
  clear();
  std::ostringstream out{};
  out << "El resultado es ........ " << resultado << '\n';
  mensaje(out.str());
}

void TerminalScreen::tabla_de_resultados(const std::vector<Resultado> &filas)
{
  constexpr std::array<int, 7> widths{7, 7, 7, 7, 9, 12, 18};
  constexpr std::array<const char *, 7> header
    {
      "Jugador", "Apuesta", "Antes", "Despues",
      "Diferenc.", "Acierto", "Notas"
    };
  std::ostringstream out{};
  crear_header(out, widths, header);
  for (const auto &fila : filas)
    crear_fila(out, fila, widths);
  mensaje(out.str());
}

void TerminalScreen::crear_fila(std::ostringstream &out,
                                const Resultado &resultado,
                                const std::array<int, 7> &width)
{
  out << "| "
      << std::setw(width[0]) << resultado.id << " | "
      << std::setw(width[1]) << resultado.apuesta << " | "
      << std::setw(width[2]) << resultado.creditos_antes << " | "
      << std::setw(width[3]) << resultado.creditos_despues << " | "
      << std::setw(width[4])
      << resultado.creditos_despues - resultado.creditos_antes << " | "
      << std::setw(width[5]);
  switch (resultado.acierto)
  {
    case Acierto::Color:
      out << "Color";
      break;
    case Acierto::Numero:
      out << "Número";
      break;
    case Acierto::Ninguno:
      out << "";
      break;
  }
  out << " | " << std::setw(width[6]);
  if (resultado.creditos_despues == 0)
    out << "Eliminado.";
  else
    out << "";
  out << " | \n";
}

void TerminalScreen::crear_header(std::ostringstream &out,
                                  const std::array<int, 7> &width,
                                  const std::array<const char *, 7> &row)
{
  out << "| ";
  for (int i{}; i < row.size(); ++i)
    out << std::setw(width[i]) << row[i] << " | ";
  out << '\n';
}

TerminalScreen::TerminalScreen(Animaciones animaciones)
  : animaciones(std::move(animaciones)) {}

void TerminalScreen::se_retira() const
{
  animar(animaciones.se_retira);
}

void TerminalScreen::clear()
{
  for (int i{}; i < 60; ++i)
    std::cout << '\n';
}

void TerminalScreen::newline()
{
  mensaje("");
}
