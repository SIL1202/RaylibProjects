#pragma once
#include<string>
#include "../include/raylib.h"

struct TextInput {
  std::string value;
  bool isActive;

  TextInput();
  // handling text input, detecting characters key pressed
  void update(); 

  bool submit();

  // delete DrawRectangle here and move it to ExpandBotton.
  // remain DrawTextEx here.
  void draw();
};


