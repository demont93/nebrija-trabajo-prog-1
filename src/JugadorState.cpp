#include "JugadorState.h"
#include "state/Impl.h"

void PreguntaSeRetira::on_enter(Jugador &context, State *from) {
  if (ask == nullptr) {
    ask = std::make_unique<AskYesOrNo<Jugador>>("Te retiras?", context.get_activo());
  }
  ask->on_enter(context, nullptr);
}

void PreguntaSeRetira::process_input(char c, Jugador &context) {
}

void PreguntaSeRetira::update(Jugador &context) {
  if (ask->is_done()) {
    if (context.is_activo()) {
      std::unique_ptr<State<Jugador>> new_state (
        static_cast<State<Jugador>*>(new PreguntaApuesta()));
      context.change_state(std::move(new_state));
    } else {
      context.change_state(nullptr);
    }
  } else {

  }
}

void PreguntaSeRetira::render() {
}

void PreguntaSeRetira::on_exit(Jugador &context) {
}

void PreguntaApuesta::on_enter(Jugador &context, State *from) {

}

void PreguntaApuesta::process_input(char c, Jugador &context) {
}

void PreguntaApuesta::update(Jugador &context) {
}

void PreguntaApuesta::render() {
}

void PreguntaApuesta::on_exit(Jugador &context) {
}

void PreguntaApuesta::enqueue_state(
  std::unique_ptr<FinishingState<Jugador>> &&new_state, Jugador& context) {
  queue_machine.enqueue(
    std::forward<std::unique_ptr<FinishingState<Jugador>>>(new_state),
      context);
}
