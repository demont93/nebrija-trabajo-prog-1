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

#include <array>
#include <algorithm>
#include <random>
#include <cassert>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <fstream>

// Crea un string con el monto en forma de dinero
// FIXME:
std::string format_dinero(int dinero) {
  return std::to_string(dinero) += dinero == 1 ? " euro" : " euros";
}

// Estructura basica de un jugador, contiene la informacion basica juntos con
// las funciones basicas necesarias pertinentes como pedir una apuesta, pedir
// la eleccion de un color etc.
struct Jugador {
  int numero; // Numero de jugador para poder decir jugador 1 o jugador 2.
  int cartera; // Cuanto dinero tiene el jugador en el momento.
  int apuesta; // Ultima apuesta hecha por el jugador.
  int ranura; // Ultimo numero elegido en la apuesta del jugador.
};

// Enumeracion para evitar usar colores como numero que puede ser confuso.
enum class ColorRanura {
  Rojo,
  Negro
};

// Le pasas un array de bools y te dice si alguno esta activo.
bool alguien_activo(const std::array<bool, 4> &activos) {
  return std::any_of(activos.begin(), activos.end(),
                     [](const auto &a) { return a; });
}

// convierte de forma uniforme los colores a numeros
ColorRanura numero_a_color(int n) {
  // El assert nos permite dar un error si le pasamos valores incorrectos a la
  // funcion. Es mejor dar un error temprano que tener un bug silencioso.
  assert(n == 0 || n == 1 && "Solo se puede obtener un color del numero 0 o 1");
  if (n == 0) return ColorRanura::Rojo;
  else return ColorRanura::Negro;
}

// Te dice el color de la ranura, como lo unico que hay que saber es si la
// ranura es par, calcula si el numero es par o no y produce el color.
ColorRanura color_de_ranura(int n) {
  assert(n >= 0 && n <= 36 && "Las ranuras solo pertenecen al rango [0,36]");
  return numero_a_color(n % 2);
}

// Producir una ranura, hay que tener cuidado de pasar la distribucion correcta.
int producir_numero_ranura(std::uniform_int_distribution<int> &dist,
                           std::random_device &rand) {
  auto ret_val{dist(rand)};
  assert(ret_val >= 0 && ret_val <= 36 &&
         "Las ranuras solo pertenecen al rango [0,36]");
  return ret_val;
}

// Los tipos posibles de acierto entre la apuesta del jugador y el resultado de
// la ruleta.
enum class Acierto {
  Numero,
  Color,
  Ninguno,
};

// Le pasas la ranura de la apuesta y le pasas la ranura que salio en la ruleta
// deberia devolver el tipo de acierto. Si el match es de numero devuelve
// Acierto::NumeroYColor. Si el match es de color solamente, devuelve
// Acierto::Color. Sino devuelve Acierto::Ninguno.
Acierto acierto(int ranura_de_ruleta, int ranura_de_apuesta) {
  if (ranura_de_apuesta == ranura_de_ruleta) {
    return Acierto::Numero;
  } else if (color_de_ranura(ranura_de_apuesta) ==
             color_de_ranura(ranura_de_ruleta)) {
    return Acierto::Color;
  } else {
    return Acierto::Ninguno;
  }
}

// Valida la apuesta de acuerdo al dinero del jugador.
bool apuesta_es_valida(int apuesta, int disponible) {
  if (apuesta > disponible) {
    std::cerr << "Haz introducido una apuesta inválida.\n"
              << "Tu dinero actual es: " << format_dinero(disponible)
              << "\nInténtalo de nuevo.\n";
    return false;
  } else if (apuesta <= 0) {
    std::cerr << "Haz introducido una apuesta inválida.\n"
              << "No puedes apostar una cantidad menor o igual a cero.\n"
              << "Intentalo de nuevo.\n";
    return false;
  } else {
    return true;
  }
}

bool ranura_es_valida(int ranura) {
  if (ranura <= 0 || ranura > 36) {
    std::cerr << "Haz introducido un inválido, las opciones para la ruleta son "
              << "cualquier numero entre 1 y 36 inclusivos.\n"
              << "Intenta de nuevo.\n";
    return false;
  } else {
    return true;
  }
}

// Obtiene un entero a partir de una linea, suponiendo que la linea solo
// consiste de un numero y espacio opcional al principio y al final.
int parse_int_de_linea(const std::string &linea) {
  std::istringstream ss{linea};
  std::string number_string{};
  std::string rest{};
  if (!(ss >> number_string))
    throw std::runtime_error("No se encontro un numero.");
  ss >> rest;
  if (!ss.eof())
    throw std::runtime_error("El numero no debe estar acompañado de nada más.");
  return std::stoi(number_string);
}

void mostrar_mensaje_eleccion_apuesta(int n_jugador) {
  std::cout << "Jugador " << n_jugador << ", introduce tu apuesta:\n>";
}

// Obtiene la apuesta de un jugador especifico y valida que la apuesta hecha
// sea posible de acuerdo al dinero que el jugador tiene.
// Si la apuesta es invalida, pide al usuario la apuesta nuevamente.
int obtener_apuesta(int n_jugador, int disponible) {
  int apuesta;
  std::string input{};
  do {
    mostrar_mensaje_eleccion_apuesta(n_jugador);
    // Error de lectura del stream y corrupcion del estado interno.
    if (!(std::getline(std::cin, input)))
      throw std::runtime_error("Ocurrio un error interno en el buffer.");

    try {
      apuesta = parse_int_de_linea(input);
    } catch (const std::invalid_argument &e) {
      std::cerr << "No se pudo interpretar un entero a partir del input.\n"
                << "Intentalo de nuevo.\n";
      continue;
    } catch (const std::out_of_range &e) {
      std::cerr << "El numero que introdujiste es demasiado grande.\n"
                << "Intentalo de nuevo.\n";
      continue;
    }
  } while (!apuesta_es_valida(apuesta, disponible));
  return apuesta;
}

// Mensaje para pedir la eleccion del jugador de la ruleta.
void mostrar_mensaje_eleccion_ruleta(int n_jugador) {
  std::cout << "Jugador " << n_jugador << ", introduce tu el numero "
            << "de la ruleta que vas a elegir.\nRecuerda que los numeros "
            << "pares son rojos y los impares negros.\n>";
}

// Se encarga de pedirle al usuario la ranura que va a elegir y valida
// la entrada. Si es invalida, la pide de nuevo.
int obtener_eleccion_ranura(int n_jugador) {
  int numero_elegido;
  std::string input{};
  do {
    mostrar_mensaje_eleccion_ruleta(n_jugador);
    if (!std::getline(std::cin, input))
      throw std::runtime_error("Ocurrio un error inesperado.");

    try {
      numero_elegido = parse_int_de_linea(input);
    } catch (const std::invalid_argument &e) {
      std::cerr << "No se pudo interpretar un entero a partir del input.\n"
                << "Intentalo de nuevo.\n";
      continue;
    } catch (const std::out_of_range &e) {
      std::cerr << "El numero que introdujiste es demasiado grande.\n"
                << "Intentalo de nuevo.\n";
      continue;
    }
  } while (!ranura_es_valida(numero_elegido));
  return numero_elegido;
}

void mostrar_bienvenida() {
  std::cout << "Bienvenido al juego, cada quien comienza con 10 euros.\n"
            << "Cada ronda puedes jugar o retirarte. Si pierdes todo estas "
            << "fuera\n";
}

template<typename F>
void para_cada_activo(std::array<bool, 4> &activos,
                      std::array<Jugador, 4> &jugadores,
                      const F &f) {
  for (int i{}; i < activos.size(); ++i) {
    if (activos[i]) {
      f(jugadores[i], activos[i]);
    }
  }
}

bool preguntar_si_continua(int n_jugador) {
  std::string input{};
  while (true) {
    std::cout << "Jugador " << n_jugador << ": "
              << "¿Continúas? (y/n)\n";

    if (!std::getline(std::cin, input))
      throw std::runtime_error("Error inesperado de IO.");
    if (input == "y") return true;
    else if (input == "n") return false;
    else {
      std::cerr << "Opcion inválida, debes introducir solamente y o n\n";
      continue;
    }
  }
}

void mostrar_resultado(int resultado) {
  std::cout << "Salio " << resultado << '\n';
}

void actualizar_jugador_con_resultado(Jugador &jugador, int resultado,
                                      bool &activo) {
  auto acierto_jugador{acierto(resultado, jugador.ranura)};
  if (acierto_jugador == Acierto::Numero) {
    std::cout << "Felicidades * 35\n";
    jugador.cartera *= 35;
  } else if (acierto_jugador == Acierto::Color) {
    std::cout << "Felicidades * 2\n";
    jugador.cartera *= 2;
  } else {
    std::cout << "Perdiste tu apuesta.\n";
    jugador.cartera -= jugador.apuesta;
    if (jugador.cartera == 0) {
      std::cout << "eliminado\n";
      activo = false;
    }
  }
}

// Debe mostrar los ganadores y perdedores + el estado final de la banca.
void mostrar_resultado_final(const std::array<Jugador, 4> &jugadores,
                             const std::array<bool, 4> &activos) {
  int banca{0};
  for (const auto &jugador : jugadores)
    banca -= jugador.cartera - 10;
  std::cout << "Banco quedó con un saldo de " << format_dinero(banca) << '\n'
            << "chao\n";
}

// Animacion girar ruleta
void animar_giro_2_cuadros(const std::string &imagen_1,
                           const std::string &imagen_2) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  time_point<system_clock> end{system_clock::now() + 3s};
  bool flip{true};
  while (system_clock::now() < end) {
    for (int n{}; n < 50; ++n) std::cout << '\n';
    std::cout << (flip ? imagen_1 : imagen_2) << '\n';
    std::this_thread::sleep_for(100ms);
    flip = !flip;
  }
}

int girar_rueda(std::uniform_int_distribution<int> &distribution,
                std::random_device &random_device,
                const std::string &imagen_1,
                const std::string &imagen_2
) {
  animar_giro_2_cuadros(imagen_1, imagen_2);
  return producir_numero_ranura(distribution, random_device);
}

std::string leer_imagen(std::ifstream &in) {
  std::string ret_val{};
  char c;
  if (in.is_open()) {
    while (in.get(c) && c != '~')
      ret_val.push_back(c);
    in.ignore();
  } else {
    throw std::runtime_error("No pude abrir el file.");
  }
  return ret_val;
}

int main() {
  // El numero del jugador sera igual a su indice.
  // Todos los jugadores empiezan con 10 euros.
  std::array<Jugador, 4> jugadores{
    Jugador{1, 10, 0, 0},
    Jugador{2, 10, 0, 0},
    Jugador{3, 10, 0, 0},
    Jugador{4, 10, 0, 0}
  };
  // Guardamos los activos en un array paralelo separado para evitar gastar
  // memoria innecesaria dentro del struct jugador.
  std::array<bool, 4> activos{true, true, true, true};

  // Motor de numeros aleatorios.
  std::random_device rand{};
  // Ranura puede ser [0,36]
  std::uniform_int_distribution ruleta(0, 36);

  // Cargar imagenes
  std::ifstream stencil_file("roulette.txt");
  // FIXME handle io errors
  const auto banner{leer_imagen(stencil_file)};
  const auto ruleta_ascii_1{leer_imagen(stencil_file)};
  const auto ruleta_ascii_2{leer_imagen(stencil_file)};
  stencil_file.close();
  const auto imagen_1{banner + ruleta_ascii_1};
  const auto imagen_2{banner + ruleta_ascii_2};

  // Bienvenida
  mostrar_bienvenida();

  // Loop de rondas
  while (alguien_activo(activos)) {
    // Obtener apuestas y ranuras.
    para_cada_activo(activos, jugadores, [](Jugador &jugador, bool &activo) {
      activo = preguntar_si_continua(jugador.numero);
      if (activo) {
        jugador.apuesta = obtener_apuesta(jugador.numero, jugador.cartera);
        jugador.ranura = obtener_eleccion_ranura(jugador.numero);
      }
    });

    // Girar la rueda
    if (alguien_activo(activos)) {
      auto resultado_ruleta{girar_rueda(ruleta, rand, imagen_1, imagen_2)};
      mostrar_resultado(resultado_ruleta);

      // Mostrar resultados de la ronda
      para_cada_activo(activos, jugadores, [=](Jugador &jugador, bool &activo) {
        actualizar_jugador_con_resultado(jugador, resultado_ruleta, activo);
      });
    }
  }

  // Resultado final para cada jugador
  mostrar_resultado_final(jugadores, activos);

  return 0;
}
