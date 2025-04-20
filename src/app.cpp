#include "include/raylib.h"
#include <iostream>
#include <vector>

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
  const char *text;
  Vector2 fontPos;
  int fontSize;

public:
  CircleBotton(Vector2 p, float r, Color c, const char *text, int fontSize)
      : pos(p), r(r), color(c), text(text), fontSize(fontSize) {}

  bool isHovered() const {
    return CheckCollisionPointCircle(GetMousePosition(), this->pos, this->r);
  }
  bool isClicked() const {
    return CheckCollisionPointCircle(GetMousePosition(), this->pos, this->r) &&
               IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
           IsKeyPressed(KEY_TAB);
  }

  void draw() const {
    DrawCircleV(this->pos, this->r, this->color);
    int textWidth = MeasureText(this->text, this->fontSize);
    int textX = this->pos.x - static_cast<float>(textWidth) / 2;
    int textY = this->pos.y - static_cast<float>(fontSize) / 2;

    DrawText(this->text, textX, textY, fontSize, WHITE);
  }
};

void BottonHoverAnimation(CircleBotton &botton) {
  float radius = 35;
  int fontSize = 18;
  botton.r = radius;
  botton.fontSize = fontSize;
}

// one thing in to-do-list
struct thing {
  int width, height;
  int x, y;
  const char *text;
  void draw() const {
    DrawRectangle(x, y, width, height, BLACK);
    int textWidth = MeasureText(text, 15);
    int textX = x + width / 2 - textWidth / 2;
    int textY = y + height / 2;

    DrawText(text, textX, textY, 15, WHITE);
  }
};

// to-do-list's ractangle
void AddRectangle(std::vector<thing> &ToDo, const char *text) {
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
      25, BLACK, "Add", 15);

  std::vector<thing> toDoList;

  while (!WindowShouldClose()) {
    // update
    // show botton animation
    if (AddBotton.isHovered()) {
      BottonHoverAnimation(AddBotton);
    } else { // reset botton position
      AddBotton.r = 25;
      AddBotton.fontSize = 15;
      AddBotton.fontPos = {GetScreenWidth() - Margin.right - AddBotton.r,
                           GetScreenWidth() - Margin.bottom - AddBotton.r};
    }

    if (AddBotton.isClicked()) {
      // BottonClickedAnimation();
      const char *inputText = "text";
      // InputString(inputText);

      std::cout << "clicked successfully!\n";
      AddRectangle(toDoList, inputText);
    }

    // drawing
    BeginDrawing();

    ClearBackground(WHITE);
    AddBotton.draw();
    for (const auto &list : toDoList)
      list.draw();
    // end of drawing
    EndDrawing();
  }

  // close
  CloseWindow();
  return 0;
}
