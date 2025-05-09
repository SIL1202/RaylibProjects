#pragma once
#include "../include/raylib.h"
#include <string>
#include <vector>

extern Font font;
Color Hex_to_deci(const std::string &color);

struct UILayout {
  const int left = 20;
  const int right = 20;
  const int top = 20;
  const int bottom = 20;
};
extern UILayout Margin;

class Botton {
private:
  Vector2 pos;
  Texture2D icon;
  std::vector<bool> mask; // true represent non transparancy
  int width, height;

public:
  Botton(const char *filename, Vector2 p);
  bool isHover();
  bool isPressed();
  void draw();
};
