#pragma once

#include <array>
#include <iostream>
#include <cassert>

// Los tipos posibles de acierto entre la Apuesta del Jugador y el resultado de
// la ruleta.
enum class Acierto
{
  Numero,
  Color,
  Ninguno,
};

// Enumeracion para evitar usar colores como id que puede ser confuso.
enum class ColorRanura
{
  Rojo,
  Negro
};

// convierte de forma uniforme los colores a numeros
ColorRanura numero_a_color(int n);

// Te dice el color de la ranura, como lo unico que hay que saber es si la
// ranura es par, calcula si el id es par o no y produce el color.
ColorRanura color_de_ranura(int n);

// Se asegura de que una ranura sea valida, es decir que no caiga fuera de la
// ruleta.
bool ranura_es_valida(int ranura);

// Le pasas la ranura de la Apuesta y le pasas la ranura que salio en la ruleta
// deberia devolver el tipo de acierto. Si el match es de id devuelve
// Acierto::NumeroYColor. Si el match es de color solamente, devuelve
// Acierto::Color. Sino devuelve Acierto::Ninguno.
Acierto acierto(int ranura_de_ruleta, int ranura_de_apuesta);
