#pragma once

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

std::map<std::string, std::vector<std::string>> load_image_map(
  std::ifstream &in);;

void print_image(const std::vector<std::string> &image, int y, int x);

void print_image_centered(const std::vector<std::string> &image, int y);

int get_center_x(int width);

void print_transparent_image(const std::vector<std::string> &image, int y,
                             int x);
