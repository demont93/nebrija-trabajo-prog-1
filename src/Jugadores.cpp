#include "Jugadores.h"
#include <iostream>

JugadorProxy::JugadorProxy(int &id, int &cartera,
                           Apuesta &apuesta,
                           bool &activo)
  : id(id), creditos(cartera), apuesta(apuesta), activo(activo)
{}

Jugadores::iterator Jugadores::begin()
{
  return iterator{this, 0};
}

Jugadores::iterator Jugadores::end()
{
  return iterator{this, static_cast<int>(jugadores.size())};
}

Jugadores::const_iterator Jugadores::begin() const
{
  return cbegin();
}

Jugadores::const_iterator Jugadores::end() const
{
  return cend();
}

Jugadores::const_iterator Jugadores::cbegin() const
{
  return const_iterator{this, 0};
}

Jugadores::const_iterator Jugadores::cend() const
{
  return const_iterator{this, static_cast<int>(jugadores.size())};
}

JugadorProxy Jugadores::make_proxy(size_t i)
{
  return JugadorProxy(jugadores[i].id,
                      jugadores[i].cartera,
                      jugadores[i].apuesta,
                      _activos[i]);
}

Jugadores::Jugadores(std::array<Jugador, cantidad_de_jugadores> jugadores,
                     std::array<bool, cantidad_de_jugadores> activos)
  : jugadores(jugadores), _activos(activos)
{}

std::vector<JugadorProxy> Jugadores::activos()
{
  std::vector<JugadorProxy> ret_val{};
  for (int i{}; i < static_cast<int>(jugadores.size()); ++i)
  {
    if (_activos[static_cast<size_t>(i)])
      ret_val.emplace_back(make_proxy(static_cast<size_t>(i)));
  }
  return ret_val;
}

int JugadorProxy::get_id() const noexcept
{
  return id;
}

int JugadorProxy::get_creditos() const noexcept
{
  return creditos;
}

bool JugadorProxy::apuesta_es_valida(int apuesta, int disponible)
{
  return apuesta <= disponible;
}

Resultado JugadorProxy::etapa_de_resultados(int resultado)
{
  auto acierto_jugador{acierto(resultado, apuesta.ranura)};
  Resultado ret_val
    {
      .id = id,
      .creditos_antes = creditos,
      .apuesta = apuesta.precio,
      .creditos_despues = {},
      .acierto = acierto_jugador
    };
  creditos -= apuesta.precio;

  if (acierto_jugador == Acierto::Numero)
    creditos += apuesta.precio * 35;
  else if (acierto_jugador == Acierto::Color)
    creditos += apuesta.precio * 2;
  else if (creditos == 0)
    activo = false;

  ret_val.creditos_despues = creditos;
  return ret_val;
}

bool JugadorProxy::get_activo() const noexcept
{ return activo; }

const Apuesta &
JugadorProxy::get_apuesta() const noexcept
{ return apuesta; }
