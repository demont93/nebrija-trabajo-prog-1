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
/// 4. Actualiza jugadores activos
///
/// Mostrar por pantalla al finalizar:
/// 1. Cuanto ha ganado/perdido la banca.
/// 2. Cuanto ha ganado/perdido cada jugador.
///
/// Etapas de cada turno:
/// Etapa 1: Mostrar data inicial del jugador actual.
/// Etapa 2: Preguntar si quiere continuar.
/// Etapa 2: Si continua, pedir apuesta y ranura.
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
#include <chrono>
#include <fstream>
#include <string_view>
#include <charconv>
#include <iterator>
#include "Animacion.h"

// Crea un string con el monto en forma de dinero
// FIXME:
std::string format_dinero(int dinero)
{
  return std::to_string(dinero) += dinero == 1 ? " euro" : " euros";
}

// TODO: incluir en jugador.
struct Apuesta
{
  int precio;
  int ranura;
};

// Estructura basica de un jugador, contiene la informacion basica junto con
// las funciones basicas necesarias pertinentes como pedir una apuesta, pedir
// la eleccion de un color etc.
struct Jugador
{
  int id; // ID <- [1,2,3,4]
  int cartera; // Cuanto dinero tiene el jugador en el momento.
  Apuesta apuesta; // Ultima apuesta hecha por el jugador.
};

// Enumeracion para evitar usar colores como id que puede ser confuso.
enum class ColorRanura
{
  Rojo,
  Negro
};

// Le pasas un array de bools y te dice si alguno esta activo.
bool alguien_activo(const std::array<bool, 4> &activos)
{
  return std::any_of(activos.begin(), activos.end(),
                     [](const auto &a) { return a; });
}

// convierte de forma uniforme los colores a numeros
ColorRanura numero_a_color(int n)
{
  // El assert nos permite dar un error si le pasamos valores incorrectos a la
  // funcion. Es mejor dar un error temprano que tener un bug silencioso.
  assert(n == 0 || n == 1 && "Solo se puede obtener un color del id 0 o 1");
  if (n == 0) return ColorRanura::Rojo;
  else return ColorRanura::Negro;
}

// Te dice el color de la ranura, como lo unico que hay que saber es si la
// ranura es par, calcula si el id es par o no y produce el color.
ColorRanura color_de_ranura(int n)
{
  assert(n >= 0 && n <= 36 && "Las ranuras solo pertenecen al rango [0,36]");
  return numero_a_color(n % 2);
}

// Los tipos posibles de acierto entre la apuesta del jugador y el resultado de
// la ruleta.
enum class Acierto
{
  Numero,
  Color,
  Ninguno,
};

// Le pasas la ranura de la apuesta y le pasas la ranura que salio en la ruleta
// deberia devolver el tipo de acierto. Si el match es de id devuelve
// Acierto::NumeroYColor. Si el match es de color solamente, devuelve
// Acierto::Color. Sino devuelve Acierto::Ninguno.
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

// Valida la apuesta de acuerdo al dinero del jugador.
bool apuesta_es_valida(int apuesta, int disponible)
{
  if (apuesta > disponible)
  {
    std::cerr << "Haz introducido una apuesta inválida.\n"
              << "Tu dinero actual es: " << format_dinero(disponible)
              << "\nInténtalo de nuevo.\n";
    return false;
  } else if (apuesta <= 0)
  {
    std::cerr << "Haz introducido una apuesta inválida.\n"
              << "No puedes apostar una cantidad menor o igual a cero.\n"
              << "Intentalo de nuevo.\n";
    return false;
  } else
  {
    return true;
  }
}

// Se asegura de que una ranura sea valida, es decir que no caiga fuera de la
// ruleta.
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

std::string_view trim(const std::string &str)
{
  auto isnotspace = [](int a) { return !isspace(a); };
  auto start{std::ranges::find_if(str, isnotspace)};
  auto rstart{
    std::find_if(str.rbegin(), std::make_reverse_iterator(start), isnotspace)};
  return std::string_view(start, rstart.base());
}

// Obtiene un entero a partir de una linea, suponiendo que la linea solo
// consiste de un id y espacio opcional al principio y al final.
// TODO: test
int parse_int_de_linea(const std::string &linea)
{
  size_t used;
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

// Prompt de la apuesta al usuario por stdout.
void mostrar_mensaje_pedir_apuesta(int n_jugador)
{
  std::cout << "Jugador " << n_jugador << ", introduce tu apuesta:\n> ";
}

template<typename T>
T &get_line(T &in, std::string &s)
{
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

// Obtiene la apuesta de un jugador especifico y valida que la apuesta hecha
// sea posible de acuerdo al dinero que el jugador tiene.
// Si la apuesta es invalida, pide al usuario la apuesta nuevamente.
int obtener_apuesta(int n_jugador, int disponible)
{
  std::string input{};
  while (true)
  {
    mostrar_mensaje_pedir_apuesta(n_jugador);
    get_line(std::cin, input);

    try
    {
      if (auto apuesta{parse_int_de_linea(input)};
        apuesta_es_valida(apuesta, disponible))
        return apuesta;
    } catch (const std::invalid_argument &e)
    {
      std::cerr << "No se pudo interpretar un entero a partir del input.\n"
                << "Intentalo de nuevo.\n";
    } catch (const std::out_of_range &e)
    {
      std::cerr << "El id que introdujiste es demasiado grande.\n"
                << "Intentalo de nuevo.\n";
    }
  }
}

// Prompt de id de la ruleta al usuario por stdout.
void mostrar_mensaje_eleccion_ruleta(int n_jugador)
{
  std::cout << "Jugador " << n_jugador << ", introduce el número "
            << "de la ruleta que vas a elegir.\nRecuerda que los numeros "
            << "pares son rojos y los impares negros.\n> ";
}

// Se encarga de pedirle al usuario la ranura que va a elegir y valida
// la entrada. Si es invalida, la pide de nuevo.
int obtener_eleccion_ranura(int n_jugador)
{
  std::string input{};
  while (true)
  {
    mostrar_mensaje_eleccion_ruleta(n_jugador);
    get_line(std::cin, input);
    try
    {
      if (auto numero_elegido{parse_int_de_linea(input)};
        ranura_es_valida(numero_elegido))
        return numero_elegido;
    } catch (const std::invalid_argument &e)
    {
      std::cerr << "No se pudo interpretar un entero a partir del input.\n"
                << "Intentalo de nuevo.\n";
    } catch (const std::out_of_range &e)
    {
      std::cerr << "El id que introdujiste es demasiado grande.\n"
                << "Intentalo de nuevo.\n";
    }
  }
}

// FIXME acomodar texto
void mostrar_bienvenida(const std::string &banner)
{
  std::cout << "\n\n" << banner << "\n\n"
            << "Bienvenido al juego, cada quien comienza con 10 euros.\n"
            << "Cada ronda puedes jugar o retirarte. Si pierdes todo estas "
            << "fuera\n";
}

template<typename F>
requires std::invocable<F, Jugador &, bool &> &&
         std::same_as<std::invoke_result_t<F, Jugador &, bool &>, void>
void para_cada_activo(std::array<bool, 4> &activos,
                      std::array<Jugador, 4> &jugadores,
                      const F &f)
{
  for (int i{}; i < activos.size(); ++i)
    if (activos[i]) f(jugadores[i], activos[i]);
}

// Pregunta al usuario con un prompt si continua el juego o se retira, muestra
// una imagen por stdout si se retira.
// Valida las entradas.
bool jugador_continua(int n_jugador)
{
  std::string input{};
  while (true)
  {
    std::cout << "Jugador " << n_jugador << ": "
              << "¿Continúas? (y/n)\n> ";

    if (!std::getline(std::cin, input))
      throw std::runtime_error("Error inesperado de IO.");

    if (input == "y")
      return true;
    else if (input == "n")
      return false;
    else
      std::cerr << "Opcion inválida, debes introducir solamente y o n\n";
  }
}

// Muestra el resultado de girar la ruleta, solamente es un string format.
void mostrar_resultado_ultimo_giro(int resultado)
{
  std::cout << "Salio " << resultado << '\n';
}

struct AnimacionesDeEtapaResultados
{
  Animacion acierto_color;
  Animacion acierto_numero;
  Animacion acierto_ninguno;
  Animacion eliminado;
};

// Determina si el jugador acerto su apuesta y que tipo de acierto tuvo.
// Luego actualiza los valores del jugador de acuerdo al tipo de acierto.
// Muestra una animacion de acuerdo al resultado.
void etapa_de_resultados(
  Jugador &jugador,
  int resultado,
  bool &activo,
  const AnimacionesDeEtapaResultados &animaciones)
{
  auto acierto_jugador{acierto(resultado, jugador.apuesta.ranura)};
  if (acierto_jugador == Acierto::Numero)
  {
    jugador.cartera *= 35;
    animar(animaciones.acierto_numero);
  } else if (acierto_jugador == Acierto::Color)
  {
    jugador.cartera *= 2;
    animar(animaciones.acierto_color);
  } else
  {
    jugador.cartera -= jugador.apuesta.precio;
    animar(animaciones.acierto_ninguno);
    if (jugador.cartera == 0)
    {
      animar(animaciones.eliminado);
      activo = false;
    }
  }
}

// Debe mostrar los ganadores y perdedores + el estado final de la banca.
void mostrar_resultado_final(const std::array<Jugador, 4> &jugadores,
                             const std::array<bool, 4> &activos)
{
  int banca{0};
  for (const auto &jugador : jugadores)
    banca -= jugador.cartera - 10;
  std::cout << "Banco quedó con un saldo de " << format_dinero(banca) << '\n'
            << "chao\n";
}

// Lee un archivo con las imagenes ASCII. Para hacerlo sencillo, cada imagen
// esta delimitada por el caracter '@'.
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

int program()
{
  // El id del jugador sera igual a su indice.
  // Todos los jugadores empiezan con 10 euros.
  std::array<Jugador, 4> jugadores{
    Jugador{.id = 1, .cartera = 10, .apuesta = {}},
    Jugador{.id = 2, .cartera = 10, .apuesta = {}},
    Jugador{.id = 3, .cartera = 10, .apuesta = {}},
    Jugador{.id = 4, .cartera = 10, .apuesta = {}},
  };
  std::array<bool, 4> activos{true, true, true, true};

  // Motor de numeros aleatorios.
  std::random_device rand{};
  // Ranura puede ser [0,36]
  std::uniform_int_distribution ruleta(0, 36);
  // generador_random_ruleta
  auto generador_random_ruleta{[&]() { return ruleta(rand); }};

  // Cargar imagenes
  std::ifstream stencil_file("roulette.txt");
  // FIXME handle io errors
  auto banner{leer_imagen(stencil_file)};
  auto ruleta_ascii_1{leer_imagen(stencil_file)};
  auto ruleta_ascii_2{leer_imagen(stencil_file)};
  auto chupon{leer_imagen(stencil_file)};
  auto gallina{leer_imagen(stencil_file)};
  stencil_file.close();

  auto imagen_1{banner + ruleta_ascii_1};
  auto imagen_2{banner + ruleta_ascii_2};

  auto animacion_espera_resultado{Animacion
    {
      .data = {std::move(imagen_1), std::move(imagen_2)},
      .interval = std::chrono::milliseconds(500),
      .times = 4
    }};
  auto animacion_se_retira{crear_animacion_default(std::move(gallina))};
  auto animacion_eliminado{crear_animacion_default(std::move(chupon))};
  auto animacion_acierta_color{
    crear_animacion_default("\nAcertaste al color.\n")};
  auto animacion_acierta_numero{
    crear_animacion_default("\nAcertaste al número.\n")};
  auto animacion_no_acierto{crear_animacion_default("\nPerdiste.\n")};
  AnimacionesDeEtapaResultados animaciones_de_etapa_resultados
    {
      .acierto_color = std::move(animacion_acierta_color),
      .acierto_numero = std::move(animacion_acierta_numero),
      .acierto_ninguno = std::move(animacion_no_acierto),
      .eliminado = std::move(animacion_eliminado)
    };

  mostrar_bienvenida(banner);

  // Loop de rondas
  while (alguien_activo(activos))
  {
    // Obtener apuestas.
    para_cada_activo(activos, jugadores, [&](Jugador &jugador, bool &activo)
    {
      if (jugador_continua(jugador.id))
      {
        jugador.apuesta = Apuesta
          {
            .precio = obtener_apuesta(jugador.id, jugador.cartera),
            .ranura = obtener_eleccion_ranura(jugador.id),
          };
        activo = true;
      } else
      {
        animar(animacion_se_retira);
        activo = false;
      }
    });

    // Girar la rueda
    if (alguien_activo(activos))
    {
      auto resultado_ruleta{generador_random_ruleta()};
      animar(animacion_espera_resultado);

      mostrar_resultado_ultimo_giro(resultado_ruleta);

      // Actualizar y mostrar resultados de cada apuesta
      para_cada_activo(activos, jugadores, [=](Jugador &jugador, bool &activo)
      {
        etapa_de_resultados(jugador, resultado_ruleta, activo,
                            animaciones_de_etapa_resultados);
      });
    }
  }

  // Resultado final para cada jugador
  mostrar_resultado_final(jugadores, activos);

  return 0;
}

int main()
{
  try
  {
    program();
  } catch (const std::runtime_error &e)
  {
    std::cerr << '\n' << e.what() << "\nCerrando...\n";
    return 0;
  } catch (...)
  {
    return 1;
  }

  return 0;
}
