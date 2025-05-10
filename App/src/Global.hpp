#pragma once
#include <raylib.h>
#include<string>


extern Font myfont;
extern Color HexToColor(const std::string &hex); 
inline float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

struct UILayout {
  const int top = 30;
  const int bottom = 30;
  const int left = 20;
  const int right = 20;
};
extern UILayout Margin;

// one thing in to-do-list
struct thing {
  float height; 
  float x, y;
  std::string text;

  void draw() const;
};

