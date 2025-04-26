#include "TextInput.hpp"
#include "Global.hpp"

TextInput::TextInput() : isActive(false) {}

// handling text input, detecting characters key pressed
void TextInput::update() {
  if (!isActive)
    return;
  int key = 0;
  while ((key = GetCharPressed()) > 0) {
    if (std::isprint(key)) {
      value += (char)key;
    }
  }

  if (IsKeyPressed(KEY_BACKSPACE) && !value.empty()) {
    value.pop_back();
  }
}

bool TextInput::submit() {
  if (!isActive)
    return false;
  if (IsKeyPressed(KEY_ENTER)) {
    isActive = false;
    return true;
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
    value.clear();
    isActive = false;
  }
  return false;
}

// delete DrawRectangle here and move it to ExpandBotton.
// remain DrawTextEx here.
void TextInput::draw() {
  if (!isActive)
    return;
  int recX = Margin.left + 25;
  int recY = GetScreenHeight() - Margin.bottom - 50;
  float textX = recX;
  float textY = GetScreenHeight() - Margin.bottom - 50;
  Vector2 textPos{textX + 10, textY + 15};
  DrawTextEx(myfont, value.c_str(), textPos, 30, 5, HexToColor("28301C"));
}
