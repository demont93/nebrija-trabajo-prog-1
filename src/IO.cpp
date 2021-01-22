#include "Out.h"

// Funcion para mostrar todos los mensajes que salen a la pantalla.
void mostrar_mensaje(const std::string &str)
{
  std::cout << '\n' << str << '\n';
}

// Muestra un simbolo para darle a entender al usuario que debe escribir una
// respuesta.
void prompt()
{
  std::cout << "> ";
}

void mostrar_status_jugador(const JugadorProxy &jugador)
{
  std::ostringstream out{};
  out << "Jugador #" << jugador.get_id() << "\nCréditos: "
      << format_dinero(jugador.get_cartera());
  mostrar_mensaje(out.str());
}

void mostrar_status_jugador(const Jugador &jugador)
{
  std::ostringstream out{};
  out << "Jugador #" << jugador.id << "\nCréditos: "
      << format_dinero(jugador.cartera);
  mostrar_mensaje(out.str());
}

// crea un string con el monto en forma de dinero
std::string format_dinero(int dinero)
{
  return std::to_string(dinero) += dinero == 1 ? " euro" : " euros";
}

// Prompt de la Apuesta al usuario por stdout.
void mostrar_mensaje_pedir_apuesta(int n_jugador)
{
  std::ostringstream out{};
  out << "Jugador " << n_jugador << ", introduce tu Apuesta:";
  mostrar_mensaje(out.str());
}
