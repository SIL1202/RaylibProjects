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

Botton::Botton(const char *filename, Vector2 p) : pos(p) {
  Image img = LoadImage(filename);
  ImageResize(&img, img.width * 0.1, img.height * 0.1);
  width = img.width;
  height = img.height;
  mask.resize(width * height, false);
  Color *pixels = LoadImageColors(img);
  for (int i = 0; i < width * height; i++)
    if (pixels[i].a > 100)
      mask[i] = true;

  UnloadImageColors(pixels);
  UnloadImage(img);
  icon = LoadTextureFromImage(img);
}

bool Botton::isHover() {
  Vector2 mouse = GetMousePosition();
  int localX = mouse.x - pos.x;
  int localY = mouse.y - pos.y;

  if (localX < 0 || localY < 0 || localX >= width || localY >= height)
    return false;
  return mask[localY * width + localX];
}

bool Botton::isPressed() {
  Vector2 mouse = GetMousePosition();
  int localX = mouse.x - pos.x;
  int localY = mouse.y - pos.y;

  if (localX < 0 || localY < 0 || localX >= width || localY >= height)
    return false;
  return mask[localY * width + localX] &&
         IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Botton::draw() {
  float scale = isHover() ? 1.2f : 1.0f;

  float offsetX = (width * scale - width) / 2;
  float offsetY = (height * scale - height) / 2;

  Vector2 drawPos = {pos.x - offsetX, pos.y - offsetY};
  DrawTextureEx(icon, drawPos, 0.0f, scale, WHITE);
}
