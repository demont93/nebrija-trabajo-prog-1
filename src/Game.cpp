#include <chrono>
#include <thread>
#include <fstream>
#include "Game.h"
#include "state/State.h"
#include "state/Impl.h"
#include "Utility.h"

Game::Game() {
  initscr();
  cbreak();
  nodelay(stdscr, true);
  noecho();
  curs_set(0);
  std::ifstream stencil_file{"../roulette.txt"};
  images_map = load_image_map(stencil_file);
  stencil_file.close();
  change_state(std::make_unique<Menu>());
}

Game::~Game() {
  curs_set(1);
  echo();
  nodelay(stdscr, false);
  nocbreak();
  endwin();
}

void Game::process_input() {
  c = getch();
  if (c != ERR)
    state.process_input(c, *this);
}

void Game::update() {
  state.update(*this);
}

void Game::render() {
  state.render();
}

void Game::change_state(std::unique_ptr<State<Game>> &&game_state) {
  state.change_state(std::forward<std::unique_ptr<State<Game>>>(game_state),
                     *this);
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
    refresh();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
  }
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
