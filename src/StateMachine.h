#pragma once

#include <vector>
#include <stack>
#include <queue>
#include "state/State.h"

template<typename T>
class FiniteStateMachine {
 public:
  FiniteStateMachine() = default;
  FiniteStateMachine(std::unique_ptr<State<T>> &&state);
  void change_state(std::unique_ptr<State<T>> &&new_state,
                    T &context);
  void update(T& context);
  void process_input(char c, T& context);
  void render();
  bool is_done();

 private:
  std::unique_ptr<State<T>> state{nullptr};
};

template<typename T>
FiniteStateMachine<T>::FiniteStateMachine(std::unique_ptr<State<T>> &&state)
  :state(std::forward(state)) {}

template<typename T>
void FiniteStateMachine<T>::change_state(std::unique_ptr<State<T>> &&new_state,
                                         T &context) {
  if (!is_done()) state->on_exit(context);
  state.swap(new_state);
  state->on_enter(context, new_state.get());
}

template<typename T>
void FiniteStateMachine<T>::update(T& context) {
  state->update(context);
}

template<typename T>
bool FiniteStateMachine<T>::is_done() {
  return state == nullptr;
}

template<typename T>
void FiniteStateMachine<T>::render() {
  if (!is_done()) state->render();
}

template<typename T>
void FiniteStateMachine<T>::process_input(char c, T &context) {
  state->process_input(c, context);
}

template<typename T>
class PushDownAutomata {
 public:
  void push(const FinishingState<T> &state, T &context);
  void update(T &context);

 private:
  std::stack<std::vector<std::unique_ptr<State<T>>>> state_stack;
};

template<typename T>
void PushDownAutomata<T>::push(const FinishingState<T> &state,
                               T &context) {
  if (!state_stack.empty()) {
    state_stack.top()->on_exit(context);
    state->on_enter(context, state_stack.top().get());
  } else {
    state->on_enter(context, nullptr);
  }
  state_stack.push(state);
}

template<typename T>
void PushDownAutomata<T>::update(T &context) {
  if (state_stack.empty()) return;
  auto &el{state_stack.top()};
  if (el->is_done()) {
    std::unique_ptr<State<T>> old{std::move(state_stack.top())};
    state_stack.pop();
    old->on_exit(context);
    if (!state_stack.empty()) {
      state_stack.top()->on_enter(context, old.get());
      state_stack.top()->update(context);
    }
  } else {
    el->update(context);
  }
}

template<typename T>
class StateQueueMachine {
 public:
  void enqueue(std::unique_ptr<FinishingState<T>> &&state, T &context);
  [[nodiscard]] bool is_done() const;
  void update(T &context);
  void process_input(char c, T& context);
  void render();

 private:
  std::queue<std::unique_ptr<FinishingState<T>>> queue{};
};

template<typename T>
void StateQueueMachine<T>::enqueue(std::unique_ptr<FinishingState<T>> &&state, T &context) {
  if (queue.empty()) state->on_enter(context, nullptr);
  queue.emplace(std::forward<std::unique_ptr<FinishingState<T>>>(state));
}

template<typename T>
void StateQueueMachine<T>::update(T &context) {
  if (queue.front()->is_done()) {
    queue.front()->on_exit(context);
    auto popped{std::move(queue.front())};
    queue.pop();
    if (!is_done()) {
      queue.front()->on_enter(popped.get(), context);
      queue.front()->update(context);
    }
  } else {
    queue.front->update(context);
  }
}

template<typename T>
bool StateQueueMachine<T>::is_done() const {
  return queue.empty();
}

template<typename T>
void StateQueueMachine<T>::process_input(char c, T &context) {
  queue.front()->process_input(c, context);
}

template<typename T>
void StateQueueMachine<T>::render() {
  if (!is_done()) queue.front()->render();
}
