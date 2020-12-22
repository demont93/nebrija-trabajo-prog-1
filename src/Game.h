#pragma once

#include <memory>
#include <optional>
#include <ncurses.h>
#include <map>
#include <string>
#include <vector>
#include "state/State.h"
#include "MessageQueue.h"
#include "StateMachine.h"

class Game {
 public:
  std::map<std::string, std::vector<std::string>> images_map;

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
  FiniteStateMachine<Game> state{};
  char c{'\0'};
};