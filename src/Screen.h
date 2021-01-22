#pragma once

#include <string>
#include "Animacion.h"
#include "Jugadores.h"
#include "Jugador.h"

class TerminalScreen
{
 public:
  static void mostrar_mensaje(const std::string &s);
  static void animar(const Animacion &animacion);
  static void mostrar_status_jugador(const JugadorProxy &jugador);
  static void mostrar_status_jugador(const Jugador &jugador);
  static void mostrar_mensaje_pedir_apuesta(int n_jugador);
  static void mostrar_mensaje_eleccion_ruleta(int n_jugador);
  static void status_final_jugador();
  static void status_final_banco();
};