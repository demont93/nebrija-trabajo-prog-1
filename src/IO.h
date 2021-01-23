#pragma once

#include <string>
#include <cassert>
#include <iostream>

template<typename T>
concept display = requires (T x)
{
  {x.display()} -> std::same_as<void>;
};

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

// crea un string con el monto en forma de dinero
std::string format_dinero(int dinero);

// Obtiene un entero a partir de una linea, suponiendo que la linea solo
// consiste de un id y espacio opcional al principio y al final.
int parse_int_de_linea(const std::string &linea);

// Corta whitespace de ambos bordes de un string, devuelve un string_view.
std::string_view trim(const std::string &str);

// Lee un archivo con las imagenes ASCII. Para hacerlo sencillo, cada imagen
// esta delimitada por el caracter '@'.
std::string leer_imagen(std::ifstream &in);

