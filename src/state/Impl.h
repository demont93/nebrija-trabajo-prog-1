#pragma once

#include "State.h"
#include "../Game.h"
#include "../MessageQueue.h"
#include "../StateMachine.h"
#include "../Jugador.h"

class Menu : public State<Game> {
 public:

  void process_input(char c, Game &context) override;
  void update(Game &context) override;

  void render() override;
  void on_enter(Game &context, State<Game> *from) override;
  void on_exit(Game &context) override;
 private:
  std::string mensaje_bienvenida{"Bienvenido a ruleta"};
};

class Rondas : public State<Game> {
 public:
  Rondas() = default;
  void process_input(char c, Game &context) override;
  void update(Game &context) override;
  void render() override;
  void on_enter(Game &context, State<Game> *from) override;
  void on_exit(Game &context) override;
  void set_condicion_salida(bool *cond_salida);
  [[nodiscard]] bool alguien_activo() const noexcept;
  void siguiente_apuesta();
  void push_msg(const Message& message);

 private:
  MessageQueue* message_queue{nullptr};
  StateQueueMachine<Rondas> state{};
  std::array<Jugador, 4> jugadores{
    Jugador{},
    Jugador{},
    Jugador{},
    Jugador{}
  };
  std::array<bool, 4> activos{true, true, true, true};
  std::random_device random_device{};
  std::uniform_int_distribution<int> dist_ruleta{0, 36};
  int jugador_actual{0};
  bool *running{nullptr};
  int intro_timer{25};
};

template<typename T>
class PopUpMessage : public State<T> {
 public:
  PopUpMessage(std::string &&message, int timer)
    : message(std::move(message)), timer{timer} {}

  PopUpMessage(std::string message, int timer)
    : message{std::move(message)}, timer{timer} {}

  void on_enter(T &game, State<T> *from) override;
  void process_input(char c, T &context) override;
  void update(T &context) override;
  void render() override;
  void on_exit(T &context) override;
  bool is_done();

 private:
  std::string message{};
  int timer{0};
};

template<typename T>
class AskYesOrNo : public FinishingState<T> {
 public:
  explicit AskYesOrNo(std::string question, bool &answer);
  void on_enter(T &context, State<T> *from) override;
  void process_input(char c, T &context) override;
  void update(T &context) override;
  void render() override;
  void on_exit(T &context) override;
  bool is_done();

 private:
  std::string question{};
  bool &_answer;
  bool done{false};
  bool invalid{false};
};

template<typename T>
void PopUpMessage<T>::on_enter(T &game, State<T> *from) {
  clear();
}

template<typename T>
void PopUpMessage<T>::process_input(char c, T &context) {
  if (c == 10) {
    timer = 0;
  }
}

template<typename T>
void PopUpMessage<T>::update(T &context) {
  if (timer > 0) --timer;
}

template<typename T>
void PopUpMessage<T>::render() {
  mvaddstr(20, 20, message.c_str());
}

template<typename T>
void PopUpMessage<T>::on_exit(T &context) {
}

template<typename T>
bool PopUpMessage<T>::is_done() {
  timer <= 0;
}

template<typename T>
AskYesOrNo<T>::AskYesOrNo(std::string question, bool &answer)
: question(std::move(question)), _answer{answer} {}

template<typename T>
void AskYesOrNo<T>::on_enter(T &context, State<T> *from) {
  clear();
}

template<typename T>
void AskYesOrNo<T>::process_input(char c, T &context) {
  if (c == 'n') {
    _answer = false;
    done = true;
  } else if (c == 'y') {
    _answer = true;
    done = true;
  }
}

template<typename T>
void AskYesOrNo<T>::update(T &context) {
  if (invalid) {
    context.push_msg(Message("Debes introducir unicamente y o n", 20, 20,20));
    invalid = false;
  }
}

template<typename T>
void AskYesOrNo<T>::render() {
  mvaddstr(20,20,question.c_str());
}

template<typename T>
void AskYesOrNo<T>::on_exit(T &context) {
}

template<typename T>
bool AskYesOrNo<T>::is_done() {
  return done;
}


