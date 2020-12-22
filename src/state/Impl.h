#pragma once

#include <random>
#include <cmath>
#include "State.h"
#include "../Game.h"
#include "../MessageQueue.h"
#include "../StateMachine.h"
#include "../Utility.h"

class Menu : public State<Game> {
 public:
  Menu() = default;
  void process_input(char c, Game &context) override;
  void update(Game &context) override;
  void render() override;
  void on_enter(Game &context, State<Game> *from) override;
  void on_exit(Game &context) override;

 private:
  struct CardAtt {
    CardAtt(int x, int y, int dir_vec, int sprite, int amp);
    int x;
    int y;
    int dir_vec;
    int sprite;
    int down_speed{4};
    double rads{0.0};
    int amp{getmaxy(stdscr) - 6};
  };

  using Image = std::vector<std::string>;
  std::string mensaje_bienvenida{"Presiona enter para empezar..."};
  int x_mensaje{get_center_x(mensaje_bienvenida.size())};
  int y_mensaje{getmaxy(stdscr) / 4 * 3};
  int x_banner{0};
  int y_banner{4};
  int counter{0};
  int floor{getmaxy(stdscr) - 6};
  std::vector<Image *> card_sprites{};
  std::vector<CardAtt> card_pos{};
  std::vector<std::string> *banner{nullptr};
  std::vector<std::string> *banner_inverted{nullptr};
  std::random_device random_engine{};
  std::uniform_int_distribution<int> card_direction{0, 1};
  std::uniform_int_distribution<int> card_speed{3, 4};
  std::uniform_int_distribution<int> height_dropped{floor/2, floor};
  std::uniform_int_distribution<int> time_interval_card_released{10, 30};
  std::uniform_int_distribution<int> card_to_generate{0, 3};
  int drop_next{gen_drop_interval()};

  int gen_drop_interval();
  int new_card_vector();
  void new_drop_interval();
  int initial_card_x(int dir_vec);
  void drop_card();
  void update_cards();
  static bool card_out_of_bounds(int x, int vec_dir);
};

struct Jugador {
  int numero{0};
  int cartera{10};
  int ranura_apostada{0};
  int cantidad_apostada{0};
  bool activo{true};

  Jugador() = default;
  explicit Jugador(int numero);
};

class Rondas : public State<Game> {
 public:
  std::array<Jugador, 4> jugadores{
    Jugador{1},
    Jugador{2},
    Jugador{3},
    Jugador{4}
  };

  Rondas() = default;
  void process_input(char c, Game &context) override;
  void update(Game &context) override;
  void render() override;
  void on_enter(Game &context, State<Game> *from) override;
  void on_exit(Game &context) override;
  [[nodiscard]] bool alguien_activo() const noexcept;
  void push_msg(const Message &message);
  void change_state(std::unique_ptr<State<Rondas>> &&new_state);
  MessageQueue *get_message_queue();

 private:
  MessageQueue *message_queue{nullptr};
  FiniteStateMachine<Rondas> state{};
};

class ExitScreen : public State<Game> {
 public:
  std::array<Jugador, 4> jugadores{};

  void on_enter(Game &context, State *from) override;
  void process_input(char c, Game &context) override;
  void update(Game &context) override;
  void render() override;
  void on_exit(Game &context) override;
};

template<typename T>
class AskYesOrNo : public FinishingState<T> {
 public:
  [[maybe_unused]] explicit AskYesOrNo(std::string question);
  void on_enter(T &context, State<T> *from) override;
  void process_input(char c, T &context) override;
  void update(T &context) override;
  void render() override;
  void on_exit(T &context) override;
  bool is_done() override;
  void set_answer(bool *answer);
  void reset();

 private:
  std::string question{};
  bool *_answer{nullptr};
  bool done{false};
};

template<typename T>
[[maybe_unused]] AskYesOrNo<T>::AskYesOrNo(std::string question)
  : question(std::move(question)) {}

template<typename T>
void AskYesOrNo<T>::on_enter(T &context, State<T> *from) {
}

template<typename T>
void AskYesOrNo<T>::process_input(char c, T &context) {
  if (c == 'n') {
    *_answer = false;
    done = true;
  } else if (c == 'y') {
    *_answer = true;
    done = true;
  } else {
    context.push_msg(Message("Debes introducir unicamente y o n", 20, 20, 20));
    reset();
  }
}

template<typename T>
void AskYesOrNo<T>::update(T &context) {}

template<typename T>
void AskYesOrNo<T>::render() {
  erase();
  mvaddch(20, 18, '>');
  mvaddstr(20, 20, question.c_str());
}

template<typename T>
void AskYesOrNo<T>::on_exit(T &context) {
  reset();
}

template<typename T>
bool AskYesOrNo<T>::is_done() {
  return done;
}

template<typename T>
void AskYesOrNo<T>::set_answer(bool *answer) {
  _answer = answer;
}

template<typename T>
void AskYesOrNo<T>::reset() {
  done = false;
}

template<typename T>
class AskInt : public FinishingState<T> {
 public:
  void on_enter(T &context, State<T> *from) override;
  void process_input(char c, T &context) override;
  void update(T &context) override;
  void render() override;
  void on_exit(T &context) override;
  bool is_done() override;
  void set_answer(int *_answer);
  void reset();
  void set_question(std::string _question);
 private:
  int *answer{nullptr};
  bool done{false};
  std::stringstream input;
  std::string question{};
};

template<typename T>
void AskInt<T>::on_enter(T &context, State<T> *from) {}

template<typename T>
void AskInt<T>::process_input(char c, T &context) {
  if (c == 10) {
    int result;
    if (!(input >> result)) {
      reset();
      context.push_msg(
        Message("No pude interpretar int, intenta de nuevo.", 20, 20, 20));
    } else {
      done = true;
      *answer = result;
    }
  } else {
    input << c;
  }
}

template<typename T>
void AskInt<T>::update(T &context) {
}

template<typename T>
void AskInt<T>::render() {
  erase();
  mvaddstr(20, 20, question.c_str());
  mvaddstr(25, 20, input.str().c_str());
}

template<typename T>
void AskInt<T>::on_exit(T &context) {
  reset();
}

template<typename T>
bool AskInt<T>::is_done() {
  return done;
}

template<typename T>
void AskInt<T>::set_answer(int *_answer) {
  answer = _answer;
}

template<typename T>
void AskInt<T>::reset() {
  done = false;
  input.clear();
  input.str("");
}

template<typename T>
void AskInt<T>::set_question(std::string _question) {
  question = std::move(_question);
}
