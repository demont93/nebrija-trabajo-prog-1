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
/// Mostrar por pantalla al finalizar:
/// 1. Cuanto ha ganado/perdido la banca.
/// 2. Cuanto ha ganado/perdido cada jugador en orden.
///
/// Etapas de cada turno:
///  Etapa 1: Mostrar data inicial del jugador actual.
///  Etapa 2: Pedir casilla o aceptar retiro.
///  Etapa 3: Simular giro de la rueda con un "sleep".
///  Etapa 4: Mostrar si ha acertado, que ha acertado
///           Si se ha quedado sin dinero.
///           Data actualizada.
///
/// Mostrar por pantalla en cada etapa de cada turno:
/// Etapa 1:
/// 1. Etiqueta del jugador. Ej. Jugador 1.
/// 2. Dinero actual. Ej. e5
/// Etapa 2:
/// 1. Casilla y color elegido.
/// Etapa 3:
/// 1. Indicador de que la rueda esta girando. (posbile animacion para flair).
/// Etapa 4:
/// 1. Si ha acertado o no y que ha acertado. Ej. Acertaste color.
/// 2. Dinero actualizado.
/// 3. Si se ha quedado sin dinero.
///
//============================================================================//

#include <array>
#include <algorithm>
#include <random>
#include <cassert>

// Enumeracion para evitar usar colores como numero que puede ser confuso.
enum class ColorRanura {
  Rojo,
  Negro
};

// Le pasas un array de bools y te dice si alguno esta activo.
bool alguien_activo(const std::array<bool, 4> &activos) {
  return std::ranges::any_of(activos, [](const auto &a) { return a; });
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
  // TODO utilizar la funcion numero a color
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
  NumeroYColor,
  Color,
  Ninguno,
};

// Le pasas la ranura de la apuesta y le pasas la ranura que salio en la ruleta
// deberia devolver el tipo de acierto. Si el match es de numero devuelve
// Acierto::NumeroYColor. Si el match es de color solamente, devuelve
// Acierto::Color. Sino devuelve Acierto::Ninguno.
Acierto acierto(int ranura_de_ruleta, int ranura_de_apuesta) {
  // TODO utiliza color_de_ranura()
}

// Calcula el siguiente turno a partir de el turno anterior y la tabla de
// jugadores activos. Incrementa el turno y calcula el modulo 4 para hacer
// "wrap-around", si el jugador no esta activo sigue al siguiente.
int siguiente_turno(int turno, const std::array<bool, 4> &activos) {
  do turno = ++turno % 4; while (!activos[turno]);
  return turno;
}

int main() {
  // El numero del jugador sera su indice.
  // Todos los jugadores empiezan con 10 euros.
  std::array<int, 4> carteras{10, 10, 10, 10};
  // Para hacerlo sencillo me parece mas facil hacer 2 arrays paralelos en vez
  // de una estructura jugador.
  std::array<bool, 4> activos{true, true, true, true};

  // Motor de numeros aleatorios.
  std::random_device rand{};
  // Ranura puede ser [0,36]
  std::uniform_int_distribution rand_ranura(0, 36);

  // El banco comienza vacio para calcular si gano o perdio.
  int banco{0};

  // Turno empieza en 0 y se reinicia al final.
  int turno{0};

  while (alguien_activo(activos)) {
    // hacer cosas...
    turno = siguiente_turno(turno, activos);
  }

  // mostrar resultado final...

  return 0;
}
