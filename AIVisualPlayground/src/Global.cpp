#include "Global.h"

Color Hex_to_deci(const std::string &color) {
  Color c;
  if (color.empty())
    return WHITE;
  if (color.length() < 6)
    return WHITE;

  c.r = std::stoi(color.substr(0, 2), nullptr, 16);
  c.g = std::stoi(color.substr(2, 2), nullptr, 16);
  c.b = std::stoi(color.substr(4, 2), nullptr, 16);
  c.a = 255;

  return c;
}

UILayout Margin;
