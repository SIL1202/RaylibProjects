#pragma once
#include "../include/raylib.h"
#include <string>

Color Hex_to_deci(const std::string &color);

struct UILayout {
  const int left = 20;
  const int right = 20;
  const int top = 20;
  const int bottom = 20;
};
extern UILayout Margin;
