#pragma once
#include "../include/raylib.h"
#include<string>

struct UILayout {
  const int top = 30;
  const int bottom = 30;
  const int left = 20;
  const int right = 20;
};

extern Font myfont;
extern UILayout Margin;
extern Color HexToColor(const std::string &hex); 
