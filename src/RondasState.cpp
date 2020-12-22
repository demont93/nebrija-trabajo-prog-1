#include "RondasState.h"
#include "JugadorState.h"

void Turnos::on_enter(Rondas &context, State *from) {
  set_jugadores(context.jugadores.begin(), context.jugadores.end());
  set_message_queue(context.get_message_queue());
  if (!is_done()) change_state(std::make_unique<PreguntaSeRetira>());
}

void Turnos::process_input(char c, Rondas &context) {
  state.process_input(c, *this);
}

void Turnos::update(Rondas &context) {
  if (is_done()) context.change_state(std::make_unique<GirarRueda>());
  else state.update(*this);
}

void Turnos::render() {
  state.render();
}

void Turnos::on_exit(Rondas &context) {
}

void Turnos::next() {
  if (!is_done()) {
    begin = find_activo(begin + 1, end);
    if (!is_done()) change_state(std::make_unique<PreguntaSeRetira>());
  }
}

void Turnos::set_jugadores(Turnos::iterator _begin, Turnos::iterator _end) {
  begin = find_activo(_begin, _end);
  end = _end;
}

Turnos::iterator Turnos::find_activo(Turnos::iterator begin,
                                     Turnos::iterator end) {
  return std::find_if(begin, end, [](const auto &jugador) {
    return jugador.activo;
  });
}

void Turnos::change_state(std::unique_ptr<State<Turnos>> &&new_state) {
  state.change_state(std::forward<std::unique_ptr<State<Turnos>>>(new_state),
                     *this);
}

void Turnos::set_message_queue(MessageQueue *_message_queue) {
  message_queue = _message_queue;
}

void Turnos::push_msg(const Message &message) {
  message_queue->enque(message);
}

bool Turnos::is_done() {
  return begin == end;
}

void GirarRueda::on_enter(Rondas &context, State *from) {
  clear();
  ultimo_giro = dist_ruleta(random_device);
}

void GirarRueda::process_input(char c, Rondas &context) {
}

void GirarRueda::update(Rondas &context) {
  if (animation_counter > 0) {
    --animation_counter;
  } else if (result_counter > 0) {
    --result_counter;
  } else {
    context.change_state(std::make_unique<CalcularAciertos>());
  }
}

void GirarRueda::render() {
  if (animation_counter > 0) {
    erase();
    if (animation_counter % 2 == 0)
      mvaddstr(20, 20, image_1.c_str());
    else
      mvaddstr(20, 20, image_2.c_str());
  } else if (result_counter > 0) {
    erase();
    mvaddstr(20, 20, std::to_string(ultimo_giro).c_str());
  }
}

void GirarRueda::on_exit(Rondas &context) {
  reset();
}

void GirarRueda::reset() {
  animation_counter = 30;
  result_counter = 15;
}

void CalcularAciertos::on_enter(Rondas &context, State *from) {
  clear();
  auto from_girar_rueda{dynamic_cast<GirarRueda *>(from)};
  jugadores = &context.jugadores;
  result = from_girar_rueda->ultimo_giro;
  actualizar_aciertos();
}

void CalcularAciertos::process_input(char c, Rondas &context) {
  if (c == 10) {
    context.change_state(std::make_unique<ActualizarEliminados>());
  }
}

void CalcularAciertos::update(Rondas &context) {
}

void CalcularAciertos::render() {
  int start_y{10};
  for (int i{}; i < aciertos.size(); ++i) {
    auto &jugador{(*jugadores)[i]};
    if (jugador.activo) {
      mvaddstr(10 + i, 10, acierto_string(i).c_str());
    }
  }
}

void CalcularAciertos::on_exit(Rondas &context) {
}

Color CalcularAciertos::numero_a_color(int n) {
  if (n % 2 == 0) return Color::Rojo;
  else return Color::Negro;
}

Acierto CalcularAciertos::calcular_acierto(int apuesta) const {
  if (apuesta == result)
    return Acierto::Numero;
  else if (numero_a_color(result) == numero_a_color(apuesta))
    return Acierto::Color;
  else
    return Acierto::Nada;
}

void CalcularAciertos::actualizar_aciertos() {
  for (int i{}; i < aciertos.size(); ++i) {
    auto &jugador{(*jugadores)[i]};
    if (jugador.activo) {
      aciertos[i] = calcular_acierto(jugador.ranura_apostada);
      actualizar_cartera(aciertos[i], jugador.cartera,
                         jugador.cantidad_apostada);
    }
  }
}

void CalcularAciertos::actualizar_cartera(Acierto acierto, int &cartera,
                                          int apuesta) {
  if (acierto == Acierto::Numero) cartera += (apuesta * 35);
  else if (acierto == Acierto::Color) cartera += (apuesta * 2);
}

std::string CalcularAciertos::acierto_string(int i) {
  auto &jugador{(*jugadores)[i]};
  std::ostringstream ss{};
  ss << "Jugador " << jugador.numero;
  if (aciertos[i] == Acierto::Numero) {
    ss << " acerto numero, ganando "
       << jugador.cantidad_apostada * 35 - jugador.cantidad_apostada
       << " euros";
  } else if (aciertos[i] == Acierto::Color) {
    ss << " acerto el color, ganando "
       << jugador.cantidad_apostada * 2 - jugador.cantidad_apostada
       << " euros";
  } else {
    ss << " no acerto nada, perdiendo " << jugador.cantidad_apostada
       << " euros";
  }
  ss << " quedando un saldo de " << jugador.cartera << " euros.";
  return ss.str();
}

void ActualizarEliminados::on_enter(Rondas &context, State *from) {
  jugadores = &context.jugadores;
  for (int i{}; i < eliminados.size(); ++i) {
    auto &jugador{(*jugadores)[i]};
    if (jugador.activo && jugador.cartera == 0)
      eliminados[i] = true;
  }
}

void ActualizarEliminados::process_input(char c, Rondas &context) {
}

void ActualizarEliminados::update(Rondas &context) {
  if (counter > 0) {
    --counter;
  } else {
    jugadores->operator[](current).activo = false;
    counter = 20;
    ++current;
    if (current >= eliminados.size()) {
      context.change_state(std::make_unique<Turnos>());
    }
  }
}

void ActualizarEliminados::render() {
  erase();
  auto &n{jugadores->operator[](current).numero};
  std::ostringstream ss{};
  ss << "Jugador " << n << " eliminado.";
  mvaddstr(20, 20, ss.str().c_str());
}

void ActualizarEliminados::on_exit(Rondas &context) {
  reset();
}

void ActualizarEliminados::reset() {
  std::fill(eliminados.begin(), eliminados.end(), 0);
  current = 0;
  counter = 10;
}
