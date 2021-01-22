#pragma once

#include <string>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include "Jugadores.h"

// Funcion para mostrar todos los mensajes que salen a la pantalla.
void mostrar_mensaje(const std::string &str);

// Muestra un simbolo para darle a entender al usuario que debe escribir una
// respuesta.
void prompt();

// getline especializado para lanzar errores.
template<typename T>
T &get_line(T &in, std::string &s)
{
  prompt();
  if (!(std::getline(in, s)))
  {
    if (std::cin.bad())
      throw std::runtime_error("Ocurrio un error interno en el buffer.");
    else if (std::cin.eof())
      throw std::runtime_error("EOF encontrado.");
    else
      assert(false && "Shouldn't reach here.");
  } else
  {
    return in;
  }
}

void mostrar_status_jugador(const JugadorProxy &jugador);

void mostrar_status_jugador(const Jugador &jugador);

// crea un string con el monto en forma de dinero
std::string format_dinero(int dinero);

// Prompt de la Apuesta al usuario por stdout.
void mostrar_mensaje_pedir_apuesta(int n_jugador);
