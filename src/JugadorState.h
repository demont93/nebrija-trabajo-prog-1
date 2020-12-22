#pragma once

#include "state/Impl.h"
#include "RondasState.h"

class PreguntaSeRetira : public State<Turnos> {
 public:
  void on_enter(Turnos &context, State *from) override;
  void process_input(char c, Turnos &context) override;
  void update(Turnos &context) override;
  void render() override;
  void on_exit(Turnos &context) override;

 private:
  Jugador *jugador{nullptr};
  AskYesOrNo<Turnos> ask {"Continuas?"};
};

class PreguntaApuesta : public State<Turnos> {
 public:
  void on_enter(Turnos &context, State *from) override;
  void process_input(char c, Turnos &context) override;
  void update(Turnos &context) override;
  void render() override;
  void on_exit(Turnos &context) override;
 private:
  AskInt<Turnos> ask{};
  int apuesta{};
};

class PreguntaNumero : public State<Turnos> {
 public:
  void on_enter(Turnos &context, State *from) override;
  void process_input(char c, Turnos &context) override;
  void update(Turnos &context) override;
  void render() override;
  void on_exit(Turnos &context) override;
 private:
  AskInt<Turnos> ask{};
  int numero{};
};
