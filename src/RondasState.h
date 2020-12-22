#pragma once

#include "state/State.h"
#include "state/Impl.h"

class Turnos : public FinishingState<Rondas> {
 public:
  std::array<Jugador, 4>::iterator begin{};
  std::array<Jugador, 4>::iterator end{};

  Turnos() = default;
  void on_enter(Rondas &context, State *from) override;
  void process_input(char c, Rondas &context) override;
  void update(Rondas &context) override;
  void render() override;
  void on_exit(Rondas &context) override;
  void change_state(std::unique_ptr<State<Turnos>> &&new_state);
  void set_message_queue(MessageQueue *_message_queue);
  void push_msg(const Message &message);
  void next();
  bool is_done() override;

 private:
  FiniteStateMachine<Turnos> state{};
  MessageQueue *message_queue{nullptr};

  using iterator = std::array<Jugador, 4>::iterator;
  static iterator find_activo(iterator begin, iterator end);
  void set_jugadores(iterator _begin, iterator _end);
};

class GirarRueda : public State<Rondas> {
 public:
  int ultimo_giro{0};

  void on_enter(Rondas &context, State *from) override;
  void process_input(char c, Rondas &context) override;
  void update(Rondas &context) override;
  void render() override;
  void on_exit(Rondas &context) override;
  void reset();
 private:
  std::random_device random_device{};
  std::uniform_int_distribution<int> dist_ruleta{0, 36};
  int animation_counter{30};
  int result_counter{15};
  std::string image_1{"-"};
  std::string image_2{"|"};
};

enum class Acierto {
  Numero,
  Color,
  Nada,
};

enum class Color {
  Rojo,
  Negro,
};

class CalcularAciertos : public State<Rondas> {
 public:
  int result{0};
  std::array<Jugador, 4> *jugadores;
  std::array<Acierto, 4> aciertos;

  void on_enter(Rondas &context, State *from) override;
  void process_input(char c, Rondas &context) override;
  void update(Rondas &context) override;
  void render() override;
  void on_exit(Rondas &context) override;

 private:
  static Color numero_a_color(int n);
  [[nodiscard]] Acierto calcular_acierto(int apuesta) const;
  void actualizar_aciertos();
  static void actualizar_cartera(Acierto acierto, int &cartera, int apuesta);
  std::string acierto_string(int i);
};

class ActualizarEliminados : public State<Rondas> {
 public:
  std::array<Jugador, 4> *jugadores;
  std::array<bool, 4> eliminados{};
  void on_enter(Rondas &context, State *from) override;
  void process_input(char c, Rondas &context) override;
  void update(Rondas &context) override;
  void render() override;
  void on_exit(Rondas &context) override;
  void reset();

 private:
  int counter{20};
  int current{0};
};
