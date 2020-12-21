#pragma once

#include <memory>
#include <random>
#include <algorithm>
#include <ncurses.h>
#include <sstream>
#include <cassert>
#include "../MessageQueue.h"

class Game;

template<typename T>
class State {
 public:
  virtual ~State() = default;
  virtual void on_enter(T &context, State *from) = 0;
  virtual void process_input(char c, T &context) = 0;
  virtual void update(T &context) = 0;
  virtual void render() = 0;
  virtual void on_exit(T &context) = 0;
};

template<typename T>
class FinishingState : public State<T> {
 public:
  virtual bool is_done() = 0;
};

