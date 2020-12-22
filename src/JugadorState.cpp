#include "JugadorState.h"
#include "state/Impl.h"

void PreguntaSeRetira::on_enter(Turnos &context, State *from) {
  jugador = context.begin;
  ask.set_answer(&context.begin->activo);
  ask.on_enter(context, nullptr);
}

void PreguntaSeRetira::process_input(char c, Turnos &context) {
  ask.process_input(c, context);
}

void PreguntaSeRetira::update(Turnos &context) {
  if (ask.is_done()) {
    if (context.begin->activo) {
      context.change_state(std::make_unique<PreguntaApuesta>());
    } else {
      context.next();
    }
  }
}

void PreguntaSeRetira::render() {
  ask.render();
  std::ostringstream ss{};
  ss << "Jugador " << jugador->numero << ": tienes "
     << jugador->cartera << " euros";
  mvaddstr(10,10, ss.str().c_str());
}

void PreguntaSeRetira::on_exit(Turnos &context) {
  ask.on_exit(context);
}

void PreguntaApuesta::on_enter(Turnos &context, State *from) {
  ask.on_enter(context, nullptr);
  ask.set_answer(&apuesta);
  ask.set_question("Introduce la cantidad a apostar.");
}

void PreguntaApuesta::process_input(char c, Turnos &context) {
  ask.process_input(c, context);
}

void PreguntaApuesta::update(Turnos &context) {
  if (ask.is_done()) {
    if (apuesta > context.begin->cartera || apuesta <= 0) {
      context.push_msg(Message("Cantidad invalida", 20, 20, 20));
      ask.reset();
    } else {
      context.begin->cantidad_apostada = apuesta;
      context.begin->cartera -= apuesta;
      context.change_state(std::make_unique<PreguntaNumero>());
    }
  }
}

void PreguntaApuesta::render() {
  ask.render();
}

void PreguntaApuesta::on_exit(Turnos &context) {
  ask.on_exit(context);
}

void PreguntaNumero::on_enter(Turnos &context, State *from) {
  ask.on_enter(context, nullptr);
  ask.set_answer(&numero);
  ask.set_question("Introduce la ranura a apostar.");
}

void PreguntaNumero::process_input(char c, Turnos &context) {
  ask.process_input(c, context);
}

void PreguntaNumero::update(Turnos &context) {
  if (ask.is_done()) {
    if (numero > 36 || numero <= 0) {
      context.push_msg(Message("Ranura invalida", 20, 20, 20));
      ask.reset();
    } else {
      context.begin->ranura_apostada = numero;
      context.next();
    }
  }
}

void PreguntaNumero::render() {
  ask.render();
}

void PreguntaNumero::on_exit(Turnos &context) {
  ask.on_exit(context);
}
