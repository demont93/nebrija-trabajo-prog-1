#include <cassert>
#include <memory>
#include "state/State.h"
#include "state/Impl.h"
#include "Jugador.h"

void Jugador::update(Rondas &context) {
  if (!is_activo()) return;
  state_machine.update(*this);
}

bool Jugador::is_activo() const {
  return activo;
}

void Jugador::calcular_acierto(int ranura_de_ruleta) {
  // if (apuesta == ranura_de_ruleta) {
  //   acierto = Acierto::Numero;
  // } else if (color_de_ranura(apuesta) ==
  //            color_de_ranura(ranura_de_ruleta)) {
  //   acierto = Acierto::Color;
  // } else {
  //   acierto = Acierto::Nada;
  // }
}

ColorRanura Jugador::numero_a_color(int n) {
  // El assert nos permite dar un error si le pasamos valores incorrectos a la
  // funcion. Es mejor dar un error temprano que tener un bug silencioso.
  // assert(n == 0 || n == 1 && "Solo se puede obtener un color del numero 0 o 1");
  // if (n == 0) return ColorRanura::Rojo;
  // else return ColorRanura::Negro;
}

ColorRanura Jugador::color_de_ranura(int n) {
  assert(n >= 0 && n <= 36 && "Las ranuras solo pertenecen al rango [0,36]");
  return numero_a_color(n % 2);
}

bool &Jugador::get_activo() {
  return activo;
}

void Jugador::change_state(std::unique_ptr<State<Jugador>> &&new_state) {
  state_machine.change_state(
    std::forward<std::unique_ptr<State<Jugador>>>(new_state), *this);
}

void Jugador::set_message_queue(MessageQueue *_message_queue) {
  message_queue = _message_queue;
}

void Jugador::push_msg(const Message &message) {
  message_queue->enque(message);
}
