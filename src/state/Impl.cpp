#include "../RondasState.h"
#include "../Utility.h"

void Menu::process_input(char c, Game &context) {
  if (c == 10)
    context.change_state(std::make_unique<Rondas>());
}

void Menu::update(Game &context) {
  if (counter % 2 == 0) {
    auto mod{counter % 24};
    if (mod == 0) counter = 0;
    if (mod < 6)++y_banner;
    else if (mod < 12) ++x_banner;
    else if (mod < 18) --y_banner;
    else --x_banner;
  }
  ++counter;
  --drop_next;
  if (drop_next <= 0) {
    drop_card();
    new_drop_interval();
  }
  update_cards();
}

void Menu::render() {
  erase();
  for (const auto &card: card_pos) {
    print_image(*card_sprites[card.sprite], card.y, card.x);
  }

  if (counter % 2 == 0) print_transparent_image(*banner, y_banner, x_banner);
  else print_transparent_image(*banner_inverted, y_banner, x_banner);

  if (counter % 24 < 12) {
    attron(A_BOLD);
    mvaddstr(y_mensaje, x_mensaje, mensaje_bienvenida.c_str());
    attroff(A_BOLD);
  } else {
    mvaddstr(y_mensaje, x_mensaje, mensaje_bienvenida.c_str());
  }
}

void Menu::on_enter(Game &context, State<Game> *from) {
  banner = &context.images_map["hero"];
  banner_inverted = &context.images_map["hero-inverted"];
  for (int i{1}; i <= 4; ++i) {
    card_sprites.push_back(
      &context.images_map[std::string("card-") + std::to_string(i)]);
  }
  x_banner = get_center_x(banner->operator[](0).size());
}

void Menu::on_exit(Game &context) {
}

int Menu::new_card_vector() {
  auto dir{card_direction(random_engine)};
  auto speed{card_speed(random_engine)};
  return dir ? speed : speed * (-1);
}

void Menu::new_drop_interval() {
  drop_next = gen_drop_interval();
}

int Menu::gen_drop_interval() {
  return time_interval_card_released(random_engine);
}

void Menu::drop_card() {
  auto card_sprite_index{card_to_generate(random_engine)};
  auto height{height_dropped(random_engine)};
  auto dir_vector{new_card_vector()};
  card_pos.emplace_back(CardAtt{initial_card_x(dir_vector), height, dir_vector,
                                card_sprite_index, height});
}

int Menu::initial_card_x(int dir_vec) {
  return dir_vec >= 0 ? static_cast<int>(card_sprites[0]->size() * (-1))
                      : static_cast<int>(getmaxx(stdscr));
}

bool Menu::card_out_of_bounds(int x, int vec_dir) {
  return (vec_dir > 0 && x > (getmaxx(stdscr))) ||
         (vec_dir <= 0 && x < -4);
}

void Menu::update_cards() {
  auto begin{card_pos.begin()};
  while (begin != card_pos.end()) {
    begin->x += begin->dir_vec;
    begin->y = floor - (abs(static_cast<int>(cos(begin->rads) * (begin->amp))));
    begin->rads += 0.2;
    if (begin->x % 4 == 0) --begin->amp;
    if (card_out_of_bounds(begin->x, begin->dir_vec)) {
      begin = card_pos.erase(begin);
    } else {
      ++begin;
    }
  }
}

Menu::CardAtt::CardAtt(int x, int y, int dir_vec, int sprite, int amp)
  : x(x), y(y), dir_vec(dir_vec), sprite(sprite), amp{amp} {}

void Rondas::on_enter(Game &context, State<Game> *from) {
  message_queue = &context.get_message_queue();
  auto ss{std::ostringstream{}};
  ss << "Bienvenido al juego, cada quien comienza con 10 euros.\n"
     << "Cada ronda puedes jugar o retirarte. Si pierdes todo estas "
     << "fuera\n";
  mvaddstr(20, 20, ss.str().c_str());
  change_state(std::make_unique<Turnos>());
}

Jugador::Jugador(int numero) : numero{numero} {}

void Rondas::process_input(char c, Game &context) {
  state.process_input(c, *this);
}

void Rondas::render() {
  state.render();
}

void Rondas::update(Game &context) {
  if (!alguien_activo()) context.change_state(std::make_unique<ExitScreen>());
  else state.update(*this);
}

void Rondas::on_exit(Game &context) {
}

bool Rondas::alguien_activo() const noexcept {
  return std::any_of(jugadores.begin(), jugadores.end(),
                     [](const auto &a) { return a.activo; });
}

void Rondas::push_msg(const Message &message) {
  message_queue->enque(message);
}

void Rondas::change_state(std::unique_ptr<State<Rondas>> &&new_state) {
  state.change_state(
    std::forward<std::unique_ptr<State<Rondas>>>(new_state), *this);
}

MessageQueue *Rondas::get_message_queue() {
  return message_queue;
}

void ExitScreen::on_enter(Game &context, State *from) {
  clear();
  auto from_rondas{dynamic_cast<Rondas *>(from)};
  jugadores = from_rondas->jugadores;
  mvaddstr(20, 20, "se acabo");
}

void ExitScreen::process_input(char c, Game &context) {
  if (c == 10) {
    context.get_condicion_salida() = false;
  }
}

void ExitScreen::update(Game &context) {
}

void ExitScreen::render() {
}

void ExitScreen::on_exit(Game &context) {
}
