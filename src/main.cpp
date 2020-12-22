//===--- Main.cpp ---=========================================================//
//
/// \file
///
/// Reglas basicas:
/// La ruleta tiene 37 ranuras (0 - 36).
/// Pares rojos, Impares negros.
/// El 0 siempre pierde.
/// Color acertado: dinero x 2
/// Numero acertado: dinero x 35
/// 4 jugadores
///
/// Orden de los jugadores:
/// Todos los jugadores juegan en orden hasta que se retiren o pierdan el dinero.
///
/// Ronda:
/// 1. Obtener todos los turnos.
/// 2. Girar la ruleta.
/// 3. Mostrar resultados.
///
/// Mostrar por pantalla al finalizar:
/// 1. Cuanto ha ganado/perdido la banca.
/// 2. Cuanto ha ganado/perdido cada jugador.
///
/// Etapas de cada turno:
/// Etapa 1: Mostrar data inicial del jugador actual.
/// Etapa 2: Pedir casilla o aceptar retiro.
///
/// Mostrar por pantalla en cada etapa de cada turno:
/// Etapa 1:
///   1. Etiqueta del jugador. Ej. Jugador 1.
///   2. Dinero actual. Ej. e5
/// Etapa 2:
///   1. Casilla y color elegido.
///
//============================================================================//

#include "Game.h"

int main() {
  Game game{};
  game.run();
  return 0;
}
