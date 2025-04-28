#include "Global.hpp"

// one thing in to-do-list
void thing::draw() const {
  int fontSize = 30;
  int textWidth = MeasureText(text.c_str(), fontSize);

  Rectangle rec{
      .x = x,
      .y = y,
      .width = (float)textWidth + 40.f,
      .height = height,
  };

  DrawRectangleRounded(rec, 1.0f, 32, HexToColor("FFC3CC"));
  float textX = x + 25.f;
  float textY = y + (height - fontSize) / 2;
  Vector2 pos{textX, textY};
  DrawTextEx(myfont, text.c_str(), pos, fontSize, 2, HexToColor("28301C"));
}
