#include <ncurses.h>
#include "Utility.h"

std::map<std::string, std::vector<std::string>> load_image_map(
  std::ifstream &in) {
  std::map<std::string, std::vector<std::string>> map{};
  char c;
  while (in >> c) {
    if (c != '@') throw std::runtime_error("Malformed stencil file.");
    std::string key{};
    while (in.peek() != '_') key.push_back(in.get());
    in.ignore();
    int x, y;
    in >> x;
    in.ignore();
    in >> y;
    in.ignore();
    auto[p, b]{map.try_emplace(std::move(key), y, std::string{})};
    std::ranges::for_each(p->second, [=](auto &s) { s.reserve(x); });
    for (auto &map_line : p->second) std::getline(in, map_line);
  }
  return map;
}

void print_image(const std::vector<std::string> &image, int y, int x) {
  int maxx, maxy;
  getmaxyx(stdscr, maxy, maxx);
  for (int i{0}; i < image.size() && (i + y) <= maxy; ++i) {
    for (int j{0}; j < image[i].size() && (j + x) <= maxx; ++j) {
      mvaddch(y+i, x+j, image[i][j]);
    }
  }
}


void print_transparent_image(const std::vector<std::string> &image, int y,
                             int x) {
  int maxx, maxy;
  getmaxyx(stdscr, maxy, maxx);
  for (int i{0}; i < image.size() && (i + y) <= maxy; ++i) {
    for (int j{0}; j < image[i].size() && (j + x) <= maxx; ++j) {
      if (image[i][j] != ' ') mvaddch(y+i, x+j, image[i][j]);
    }
  }
}

void print_image_centered(const std::vector<std::string> &image, int y) {
  auto maxx{getmaxx(stdscr)};
  int print_at{(maxx - static_cast<int>(image[0].size())) / 2};
  print_image(image, y, print_at);
}

int get_center_x(int width) {
  auto maxx{getmaxx(stdscr)};
  return (maxx - width) / 2;
}
