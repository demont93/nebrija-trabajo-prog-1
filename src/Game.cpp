#include <chrono>
#include <thread>
#include "Game.h"
#include "state/State.h"
#include "state/Impl.h"

Game::Game() {
  initscr();
  cbreak();
  nodelay(stdscr, true);
  std::unique_ptr<State<Game>> new_state{static_cast<State<Game>*>(new Menu())};
  change_state(std::move(new_state));
}

Game::~Game() {
  endwin();
}

void Game::process_input() {
  c = getch();
  state->process_input(c, *this);
}

void Game::update() {
  state->update(*this);
}

void Game::render() {
  state->render();
}

void Game::change_state(std::unique_ptr<State<Game>> &&game_state) {
  state->on_exit(*this);
  state.swap(game_state);
  state->on_enter(*this, game_state.get());
}

void Game::run() {
  while (running) {
    if (!message_queue.empty()) {
      message_queue.update();
      message_queue.render();
    } else {
      process_input();
      update();
      render();
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
  }
  state->on_exit(*this);
}

bool &Game::get_condicion_salida() {
  return running;
}

void Game::push_msg(const Message &message) {
  message_queue.enque(message);
}

MessageQueue &Game::get_message_queue() {
  return message_queue;
}
