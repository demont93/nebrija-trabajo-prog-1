#pragma once

#include "Jugador.h"
#include "TablaResultados.h"
#include "Screen.h"
#include "IO.h"
#include "Acierto.h"
#include <array>
#include <numeric>

class JugadorProxy
{
 public:
  JugadorProxy(int &id, int &cartera, Apuesta &apuesta, bool &activo);

  template<Screen S>
  void etapa_inicial(const S &screen);

  Resultado etapa_de_resultados(int resultado);

  [[nodiscard]] int get_id() const noexcept;

  [[nodiscard]] int get_creditos() const noexcept;

  [[nodiscard]] const Apuesta &get_apuesta() const noexcept;

  [[nodiscard]] bool get_activo() const noexcept;

 private:
  int &id;
  int &creditos;
  Apuesta &apuesta;
  bool &activo;

  template<Screen S>
  void obtener_apuesta();

  template<Screen S>
  bool continua();

  template<Screen S>
  [[nodiscard]] int obtener_dinero_apuesta() const;

  static bool apuesta_es_valida(int apuesta, int disponible);

  // Se encarga de pedirle al usuario la ranura que va a elegir y valida
  // la entrada. Si es invalida, la pide de nuevo.
  template<Screen S>
  int obtener_eleccion_ranura(int n_jugador);
};

class Jugadores
{
 private:
  static constexpr const int cantidad_de_jugadores{4};

  template<typename Container>
  requires std::same_as<std::remove_cvref_t<Container>, Jugadores>
  struct Iter;

 public:
  using iterator = Iter<Jugadores>;
  using const_iterator = Iter<const Jugadores>;

 public:
  Jugadores(std::array<Jugador, cantidad_de_jugadores> jugadores,
            std::array<bool, cantidad_de_jugadores> activos);

  iterator begin();

  iterator end();

  [[nodiscard]] const_iterator begin() const;

  [[nodiscard]] const_iterator end() const;

  [[nodiscard]] const_iterator cbegin() const;

  [[nodiscard]] const_iterator cend() const;

  std::vector<JugadorProxy> activos();

  template<Screen S>
  void etapa_final(S &screen);

 private:
  std::array<Jugador, cantidad_de_jugadores> jugadores;
  std::array<bool, cantidad_de_jugadores> _activos;

  JugadorProxy make_proxy(size_t i);
};

template<Screen S>
bool JugadorProxy::continua()
{
  std::string input{};
  while (true)
  {
    S::mensaje("¿Continúas? (y/n)");
    get_line(std::cin, input);

    if (input == "y")
      return true;
    else if (input == "n")
      return false;
    else
      std::cerr << "Opcion inválida, debes introducir solamente y o n\n";
  }
}

template<Screen S>
void JugadorProxy::etapa_inicial(const S &screen)
{
  S::mensaje("");
  S::status_jugador(*this);
  if (continua<S>())
  {
    obtener_apuesta<S>();
    if (apuesta.precio == creditos)
      S::mensaje("*-*-*-*-*- BUENA SUERTE -*-*-*-*-*");
    else
      S::mensaje("*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*");
  } else
  {
    screen.se_retira();
    activo = false;
  }
}

template<Screen S>
void Jugadores::etapa_final(S &screen)
{
  // Debe mostrar los ganadores y perdedores + el estado final de la banca.
  int banca{std::accumulate(
    jugadores.begin(),
    jugadores.end(),
    0,
    [](int acc, const Jugador &jugador)
    { return acc + 10 - jugador.cartera; }
  )};
  screen.resultado_final(*this, banca);
}

// Se encarga de pedirle al usuario la ranura que va a elegir y valida
// la entrada. Si es invalida, la pide de nuevo.
template<Screen S>
int JugadorProxy::obtener_eleccion_ranura(int n_jugador)
{
  std::string input{};
  while (true)
  {
    S::mensaje_eleccion_ruleta(*this);
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

template<Screen S>
void JugadorProxy::obtener_apuesta()
{
  apuesta =
    {
      .precio = obtener_dinero_apuesta<S>(),
      .ranura = obtener_eleccion_ranura<S>(id),
    };
}

template<Screen S>
int JugadorProxy::obtener_dinero_apuesta() const
{
  std::string input{};
  while (true)
  {
    S::mensaje_pedir_apuesta(*this);
    get_line(std::cin, input);
    try
    {
      if (auto nueva_apuesta{parse_int_de_linea(input)};
        apuesta_es_valida(nueva_apuesta, creditos))
        return nueva_apuesta;
      else
        std::cerr << "No tienes suficientes créditos para hacer esa apuesta.\n"
                     "Inténtalo de nuevo.\n";
    } catch (const std::invalid_argument &e)
    {
      std::cerr << "No se pudo interpretar un entero a partir del input.\n"
                   "Inténtalo de nuevo.\n";
    } catch (const std::out_of_range &e)
    {
      std::cerr << "El num que introdujiste es demasiado grande.\n"
                   "Inténtalo de nuevo.\n";
    }
    std::cerr.flush();
  }
}

template<typename Container>
requires std::same_as<std::remove_cvref_t<Container>, Jugadores>
struct Jugadores::Iter
{
  using container = typename std::remove_reference<Container>::type;
  using difference_type = int64_t;
  using value_type = JugadorProxy;

  Iter() = default;

  size_t index{0};
  container *data{nullptr};

  JugadorProxy operator*() const;

  Iter &operator++();

  Iter &operator++(int);

  bool operator==(const Iter &rhs) const
  {
    return data == rhs.data && index == rhs.index;
  }

 private:
  friend Jugadores;

  Iter(container *data, int index);
};

template<typename Container>
requires std::same_as<std::remove_cvref_t<Container>, Jugadores>
JugadorProxy Jugadores::Iter<Container>::operator*() const
{
  return data->make_proxy(index);
}

template<typename Container>
requires std::same_as<std::remove_cvref_t<Container>, Jugadores>
Jugadores::Iter<Container> &Jugadores::Iter<Container>::operator++()
{
  ++index;
  return *this;
}

template<typename Container>
requires std::same_as<std::remove_cvref_t<Container>, Jugadores>
Jugadores::Iter<Container>::Iter(container *data, int index)
  : index(static_cast<size_t>(index)), data(data)
{}

template<typename Container>
requires std::same_as<std::remove_cvref_t<Container>, Jugadores>
Jugadores::Iter<Container> &Jugadores::Iter<Container>::operator++(int)
{
  auto old{this};
  ++index;
  return old;
}


