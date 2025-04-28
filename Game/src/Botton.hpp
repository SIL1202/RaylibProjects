#pragma once
#include "../include/raylib.h"
#include <memory>
#include<string>
#include<vector>
#include<cctype>
#include<iostream>
#include "Global.hpp"

class TextureRAII{
private:
  Texture2D texture;
public:
  TextureRAII(const char*);
  ~TextureRAII();

  Texture2D get() const;
};

class CircleBotton {
public:
  Vector2 pos;
  float r;
  Color color; // for original botton color
  Vector2 fontPos;
  float rotation;

public:
  std::unique_ptr<TextureRAII> icon;// 在 CircleBotton 類中禁用複製
  CircleBotton(const CircleBotton&) = delete;
  CircleBotton& operator=(const CircleBotton&) = delete;

  CircleBotton(Vector2 p, float r, Color c, const char *iconPath, float initRotation = 0.0f);
  CircleBotton(Vector2 p, float r, Color c);

  bool isHovered() const;
  bool isClicked() const;

  virtual void update();
  // drawing original circle botton and texture icon
  virtual void draw() ; 

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
  void update() override; 

  void triggerExpand();

  void triggerClose(); 

  // drawing rounded ractangle
  void draw() override; 
};
