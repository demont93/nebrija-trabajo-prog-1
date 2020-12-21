#pragma once

#include <memory>
#include <optional>
#include <ncurses.h>
#include "state/State.h"
#include "MessageQueue.h"

class Game {
 public:
  Game();
  ~Game();
  void run();
  void process_input();
  void update();
  void render();
  bool &get_condicion_salida();
  void push_msg(const Message& message);
  MessageQueue& get_message_queue();
  void change_state(std::unique_ptr<State<Game>> &&game_state);

 private:
  MessageQueue message_queue{};
  bool running{true};
  std::unique_ptr<State<Game>> state{nullptr};
  char c{'\0'};
};