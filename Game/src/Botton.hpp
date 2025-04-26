#pragma once
#include "../include/raylib.h"
#include<string>
#include<vector>
#include<cctype>
#include<iostream>

class CircleBotton {
public:
  Vector2 pos;
  float r;
  Color color; // for original botton color
  Vector2 fontPos;

public:
  Texture2D icon;

  CircleBotton(Vector2 p, float r, Color c, const char *iconPath);

  bool isHovered() const;
  bool isClicked() const;

  // drawing original circle botton and texture icon
  virtual void draw() const; 

  ~CircleBotton();
};

class ExpandBotton : public CircleBotton {
private:
  float targetWidth;
  float currentWidth;
  bool isExpanding;
  bool isClosing;

public:
  bool isExpanded;
  ExpandBotton(Vector2 p, float r, Color c, const char *iconPath,
               float expandWidth);

  // update expanding animation
  void update(); 

  void triggerExpand();

  void triggerClose(); 

  // drawing rounded ractangle
  void draw() const override; 
};

