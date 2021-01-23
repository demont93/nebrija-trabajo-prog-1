#include "IO.h"
#include <charconv>
#include <fstream>
#include <algorithm>

// Muestra un simbolo para darle a entender al usuario que debe escribir una
// respuesta.
void prompt()
{
  std::cout << "> ";
}

// crea un string con el monto en forma de dinero
std::string format_dinero(int dinero)
{
  return std::to_string(dinero) += dinero == 1 ? " euro" : " euros";
}

// Obtiene un entero a partir de una linea, suponiendo que la linea solo
// consiste de un id y espacio opcional al principio y al final.
int parse_int_de_linea(const std::string &linea)
{
  auto view = trim(linea);
  std::int64_t value;
  auto res{std::from_chars(view.begin(), view.end(), value)};

  if (res.ec == std::errc::invalid_argument ||
      res.ptr != view.end())
    throw std::invalid_argument("Invalid argument to parse int.");
  else if (res.ec == std::errc::result_out_of_range ||
           value > std::numeric_limits<int>::max() ||
           value < std::numeric_limits<int>::min())
    throw std::out_of_range("Number is out of range of int32_t.");

  return value;
}

// Corta whitespace de ambos bordes de un string, devuelve un string_view.
std::string_view trim(const std::string &str)
{
  auto isnotspace = [](int a) { return !isspace(a); };
  auto start{std::ranges::find_if(str, isnotspace)};
  auto rstart{
    std::find_if(str.rbegin(), std::make_reverse_iterator(start), isnotspace)};
  return std::string_view(start, rstart.base());
}

std::string leer_imagen(std::ifstream &in)
{
  std::string ret_val{};
  char c;
  if (in.is_open())
  {
    while (in.get(c) && c != '@')
      ret_val.push_back(c);
    in.ignore();
  } else
  {
    throw std::runtime_error("No pude abrir el file.");
  }
  return ret_val;
}

