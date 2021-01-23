#pragma once

#include "Animacion.h"
#include "Jugador.h"
#include "TablaResultados.h"
#include <string>
#include <sstream>
#include <ranges>

class JugadorProxy;

template<typename S>
concept Screen =
requires(S s)
{
  {
  S::mensaje(std::declval<const std::string &>())
  } -> std::same_as<void>;
  {
  S::status_jugador(std::declval<const JugadorProxy &>())
  } -> std::same_as<void>;
  {
  S::status_final_jugador(std::declval<const JugadorProxy &>())
  } -> std::same_as<void>;
  {
  S::mensaje_pedir_apuesta(std::declval<const JugadorProxy &>())
  } -> std::same_as<void>;
  {
  S::mensaje_eleccion_ruleta(std::declval<const JugadorProxy &>())
  } -> std::same_as<void>;
  {
  S::status_final_jugador(std::declval<const JugadorProxy &>())
  } -> std::same_as<void>;
  {
  S::resultado_final_banco(std::declval<int>())
  } -> std::same_as<void>;
  {
  s.bienvenida()
  } -> std::same_as<void>;
  {
  s.resultado_ultimo_giro(std::declval<int>())
  } -> std::same_as<void>;
  {
  S::tabla_de_resultados(std::declval<std::vector<Resultado>>())
  } -> std::same_as<void>;
  { s.se_retira() } -> std::same_as<void>;
  {
  s.resultado_final(std::declval<const std::vector<JugadorProxy> &>(),
                     std::declval<int>())
  } -> std::same_as<void>;
};

struct AnimacionesDeEtapaResultados
{
  Animacion acierto_color;
  Animacion acierto_numero;
  Animacion acierto_ninguno;
  Animacion eliminado;
};

struct Animaciones
{
  AnimacionesDeEtapaResultados etapa_de_resultados;
  Animacion se_retira;
  Animacion espera_resultado;
  Animacion splash_screen_1;
  Animacion splash_screen_2;
  Animacion trofeo;
};

