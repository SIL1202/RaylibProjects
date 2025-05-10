#pragma once
#include <raylib.h>
#include <string>
#include <vector>
// vertex color
#define COLOR_UNVISITED LIGHTGRAY
#define COLOR_VISITED Hex_to_deci("478058")
#define COLOR_RESET Hex_to_deci("B87FA7")
#define COLOR_NEXT Hex_to_deci("478058")

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
  Rectangle rectangle;

public:
  Botton(const char *filename, Vector2 p);
  Botton(Rectangle rec);
  bool isHover_tri();
  bool isPressed_tri();
  bool isHover_rec();
  bool isPressed_rec();
  void draw_tri();
  void draw_rec();
};
