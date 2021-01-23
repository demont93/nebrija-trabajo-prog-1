#pragma once

#include "Animacion.h"

struct Apuesta
{
  int precio;    // el monto en dinero de la Apuesta.
  int ranura;    // la ranura de la ruleta elegida.
};

// estructura basica de un Jugador, contiene la informacion basica junto con
// las funciones basicas necesarias pertinentes como pedir una Apuesta, pedir
// la eleccion de un color etc.
struct Jugador
{
  int id;          // ID <- [1,2,3,4]
  int cartera;     // Cuanto dinero tiene el Jugador en el momento.
  Apuesta apuesta; // Ultima Apuesta hecha por el Jugador.
};
