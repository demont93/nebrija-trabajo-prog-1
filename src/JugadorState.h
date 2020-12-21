#pragma once

#include "Jugador.h"
#include "state/Impl.h"

class PreguntaSeRetira : public State<Jugador> {
 public:
  void on_enter(Jugador &context, State *from) override;
  void process_input(char c, Jugador &context) override;
  void update(Jugador &context) override;
  void render() override;
  void on_exit(Jugador &context) override;
 private:
  std::unique_ptr<AskYesOrNo<Jugador>> ask {nullptr};
};

class PreguntaApuesta : public State<Jugador> {
 public:
  void on_enter(Jugador &context, State *from) override;
  void process_input(char c, Jugador &context) override;
  void update(Jugador &context) override;
  void render() override;
  void on_exit(Jugador &context) override;
  void enqueue_state(std::unique_ptr<FinishingState<Jugador>>&& new_state, Jugador &context);

 private:
  StateQueueMachine<Jugador> queue_machine{};
};

