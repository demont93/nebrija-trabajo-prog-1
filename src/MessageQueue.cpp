#include <ncurses.h>
#include "MessageQueue.h"

Message::Message(std::string message, int duration, int x, int y)
  : message(std::move(message)), duration(duration), x(x), y(y) {}

void MessageQueue::enque(const Message &message) {
  queue.push_back(message);
}

void MessageQueue::pop() {
  queue.pop_front();
}

void MessageQueue::render() {
  auto &el{queue.front()};
  mvaddstr(el.y, el.x, el.message.c_str());
}

bool MessageQueue::empty() {
  return queue.empty();
}

void MessageQueue::update() {
  if (counter >= queue.front().duration) {
    counter = 0;
    pop();
  } else {
    ++counter;
  }
}
