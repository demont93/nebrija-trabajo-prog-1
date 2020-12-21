#include "Impl.h"
#include "../Game.h"

void Menu::process_input(char c, Game &context) {
  if (c == 10) {
    auto new_state{
      std::unique_ptr<State<Game>>(
        static_cast<State<Game> *>(new Rondas()))};
    context.change_state(std::move(new_state));
  }
}

void Menu::update(Game &context) {}

void Menu::render() {
  mvaddstr(20, 20, mensaje_bienvenida.c_str());
}

void Menu::on_enter(Game &context, State<Game> *from) {
  mvaddstr(20, 20, mensaje_bienvenida.c_str());
}

void Menu::on_exit(Game &context) {
  clear();
}

void Rondas::on_enter(Game &context, State<Game> *from) {
  set_condicion_salida(&context.get_condicion_salida());
  message_queue = &context.get_message_queue();
  for (int i{0}; i < jugadores.size(); ++i)
    jugadores[i].set_message_queue(message_queue);
  // std::cout << "\n\n" << banner << "\n\n"
  auto ss{std::ostringstream{}};
  ss << "Bienvenido al juego, cada quien comienza con 10 euros.\n"
     << "Cada ronda puedes jugar o retirarte. Si pierdes todo estas "
     << "fuera\n";
  mvaddstr(20, 20, ss.str().c_str());
}

void Rondas::process_input(char c, Game &context) {
}

void Rondas::render() {
}

void Rondas::update(Game &context) {
}

void Rondas::on_exit(Game &context) {
  clear();
}

bool Rondas::alguien_activo() const noexcept {
  return std::any_of(activos.begin(), activos.end(),
                     [](const auto &a) { return a; });
}

void Rondas::set_condicion_salida(bool *cond_salida) {
  running = cond_salida;
}

void Rondas::push_msg(const Message &message) {
  message_queue->enque(message);
}

void Rondas::siguiente_apuesta() {
  ++jugador_actual;
}
