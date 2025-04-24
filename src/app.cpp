#include "include/raylib.h"
#include <cctype> // std::isprint()
#include <iostream>
#include <string>
#include <vector>

Font myfont;
void InitFont() {
  myfont = LoadFont("assets/JetBrainsMonoNerdFont-Bold.ttf");
  if (myfont.texture.id == 0) {
    std::cerr << "❌ Failed to load font!" << std::endl;
    exit(1);
  }
}

Color HexToColor(const std::string &hex) {
  if (hex.size() != 6 && hex.size() != 8) {
    std::cerr << "❌ Hex color format error: " << hex << "\n";
    return WHITE;
  }

  unsigned int r = std::stoi(hex.substr(0, 2), nullptr, 16);
  unsigned int g = std::stoi(hex.substr(2, 2), nullptr, 16);
  unsigned int b = std::stoi(hex.substr(4, 2), nullptr, 16);
  unsigned int a =
      (hex.size() == 8) ? std::stoi(hex.substr(6, 2), nullptr, 16) : 255;

  return Color{(unsigned char)r, (unsigned char)g, (unsigned char)b,
               (unsigned char)a};
}

struct UILayout {
  const int top = 30;
  const int bottom = 30;
  const int left = 20;
  const int right = 20;
} Margin;

class CircleBotton {
public:
  Vector2 pos;
  float r;
  Color color;
  Vector2 fontPos;

public:
  Texture2D icon;

  CircleBotton(Vector2 p, float r, Color c, const char *iconPath)
      : pos(p), r(r), color(c) {
    icon = LoadTexture(iconPath);
    icon.width = icon.width * 0.0625;
    icon.height = icon.height * 0.0625;
  }

  bool isHovered() const {
    return CheckCollisionPointCircle(GetMousePosition(), this->pos, this->r);
  }
  bool isClicked() const {
    return CheckCollisionPointCircle(GetMousePosition(), this->pos, this->r) &&
               IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
           IsKeyPressed(KEY_TAB);
  }
  /* int textWidth = MeasureText(this->text.c_str(), this->fontSize);
  float textX = this->pos.x - static_cast<float>(textWidth) / 2;
  float textY = this->pos.y - static_cast<float>(fontSize) / 2;
  Vector2 pos{textX, textY};

  DrawTextEx(myfont, this->text.c_str(), pos, fontSize, 2, WHITE); */
  void draw() const {
    DrawCircleV(pos, r, color);
    if (icon.id != 0)
      DrawTexture(icon, pos.x - (float)icon.width / 2,
                  pos.y - (float)icon.height / 2, WHITE);
  }

  ~CircleBotton() { UnloadTexture(icon); }
};

// void BottonHoverAnimation(CircleBotton &botton) {}

// one thing in to-do-list
struct thing {
  int width, height;
  int x, y;
  std::string text;
  void draw() const {
    DrawRectangle(x, y, width, height, BLACK);
    int textWidth = MeasureText(text.c_str(), 15);
    float textX = x + (float)width / 2 - (float)textWidth / 2;
    float textY = y + (float)height / 2;
    Vector2 pos{textX, textY};
    DrawTextEx(myfont, text.c_str(), pos, 15, 2, WHITE);
  }
};

struct TextInput {
  std::string value;
  bool isActive = false;

  void update() {
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

  bool submit() {
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

  void draw() {
    if (!isActive)
      return;
    float x = (float)GetScreenWidth() / 2;
    float y = GetScreenHeight() - Margin.bottom - 0;
    Vector2 pos{x + 10, y + 15};
    DrawRectangle(x, y, 200, 30, HexToColor("CFD1C4"));
    DrawTextEx(myfont, value.c_str(), pos, 15, 2, WHITE);
  }
};

// to-do-list's ractangle
void AddRectangle(std::vector<thing> &ToDo, std::string text) {
  if (!ToDo.empty()) {
    if (ToDo[ToDo.size() - 1].y + 50 >= GetScreenHeight() - Margin.bottom) {
      return;
    }
    thing todo{.width = 200,
               .height = 40,
               .x = ToDo[ToDo.size() - 1].x,
               .y = ToDo[ToDo.size() - 1].y + 50,
               .text = text};
    ToDo.emplace_back(todo);
    return;
  }
  // int X = 750 -
  ToDo.emplace_back(
      thing{200, 40, GetScreenWidth() / 2 - 200 / 2, Margin.top, text});
}

int main() {
  // initial
  InitWindow(800, 600, "To-Do-List");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  CircleBotton AddBotton(
      {static_cast<float>(GetScreenWidth() - Margin.right - 25),
       static_cast<float>(GetScreenHeight() - Margin.bottom - 25)},
      25, HexToColor("A8B2AA"), "assets/PlusIcon.png");

  std::vector<thing> toDoList;
  TextInput inputBox;

  while (!WindowShouldClose()) {
    // update
    // show botton animation
    if (AddBotton.isHovered()) {
      AddBotton.color = HexToColor("BBBEB5");
    } else { // reset botton position
      AddBotton.color = HexToColor("A8B2AA");
    }

    if (AddBotton.isClicked())
      inputBox.isActive = true;

    inputBox.update();
    if (inputBox.submit()) {
      AddRectangle(toDoList, inputBox.value.c_str());
      inputBox.value.clear();
    }

    // drawing
    BeginDrawing();

    ClearBackground(HexToColor("A8B2AA"));
    AddBotton.draw();
    for (const auto &list : toDoList)
      list.draw();

    inputBox.draw();
    // end of drawing
    EndDrawing();
  }

  // close
  CloseWindow();
  return 0;
}
