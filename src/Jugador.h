#pragma once

#include "state/State.h"
#include "StateMachine.h"

enum class Acierto {
  Numero,
  Color,
  Nada,
};

enum class ColorRanura {
  Rojo,
  Negro
};

class Rondas;

class Jugador {
 public:
  Jugador() = default;
  void set_message_queue(MessageQueue* message_queue);
  void update(Rondas &context);
  [[nodiscard]] bool is_activo() const;
  bool &get_activo();
  void change_state(std::unique_ptr<State<Jugador>> &&new_state);
  void push_msg(const Message& message);

 private:
  bool activo{true};
  int numero{0};
  int cartera{10};
  int ranura_apostada{0};
  int cantidad_apostada{0};
  Acierto acierto{Acierto::Nada};
  FiniteStateMachine<Jugador> state_machine{FiniteStateMachine<Jugador>{}};
  MessageQueue *message_queue{nullptr};

  void calcular_acierto(int ranura_de_ruleta);
  static ColorRanura numero_a_color(int n);
  static ColorRanura color_de_ranura(int n);
};
