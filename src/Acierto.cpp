#include "Acierto.h"

// Calcula el tipo de acierto.
Acierto acierto(int ranura_de_ruleta, int ranura_de_apuesta)
{
  if (ranura_de_apuesta == ranura_de_ruleta)
    return Acierto::Numero;
  else if (color_de_ranura(ranura_de_apuesta) ==
           color_de_ranura(ranura_de_ruleta))
    return Acierto::Color;
  else
    return Acierto::Ninguno;
}

ColorRanura numero_a_color(int n)
{
  // El assert nos permite dar un error si le pasamos valores incorrectos a la
  // funcion. Es mejor dar un error temprano.
  assert((n == 0 || n == 1) && "Solo se puede obtener un color del id 0 o 1");
  if (n == 0) return ColorRanura::Rojo;
  else return ColorRanura::Negro;
}

bool ranura_es_valida(int ranura)
{
  if (ranura <= 0 || ranura > 36)
  {
    std::cerr << "Seleccion " << ranura
              << " inválida, las opciones para la rule"
                 "ta son cualquier id entre 1 y 36 inclusivos.\n"
                 "Intenta de nuevo.\n";
    return false;
  } else
  {
    return true;
  }
}

// Calcula el color de una ranura a partir de su numero.
ColorRanura color_de_ranura(int n)
{
  assert(n >= 0 && n <= 36 && "Las ranuras solo pertenecen al rango [0,36]");
  return numero_a_color(n % 2);
}
