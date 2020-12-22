#pragma once

#include <string>
#include <deque>

struct Message {
  std::string message;
  int duration;
  int x;
  int y;

  Message(std::string message, int duration, int x, int y);
};

class MessageQueue {
 public:
  void enque(const Message& message);
  void pop();
  void update();
  void render();
  bool empty();
 private:
  std::deque<Message> queue{};
  int counter{0};
};
