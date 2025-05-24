#include "raylib.h"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
#define WIDTH 800
#define HEIGHT 900

class game {
public:
  enum class State { start, end };
  static State state;
};

game::State game::state = game::State::end;

class Drawable {
public:
  virtual void draw() const = 0; // abstract function
  virtual ~Drawable() = default; // base destructor
};

class Button : public Drawable {
private:
  Vector2 vec;
  float rad;
  Color col;
  Rectangle rec;

public:
  Button() {}
  Button(Vector2 v, float r, Color c) : vec(v), rad(r), col(c) {
    rec = {vec.x - r, vec.y - r, r * 2, r * 2};
  }

  void update() {
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      if (game::state == game::State::end &&
          CheckCollisionPointCircle(mouse, vec, rad)) {
        game::state = game::State::start;
      } else if (game::state == game::State::start &&
                 CheckCollisionPointRec(mouse, rec)) {
        game::state = game::State::end;
      }
    }
  }
  void draw() const override {
    if (game::state == game::State::end)
      DrawCircle(vec.x, vec.y, rad, col);
    else
      DrawRectangle(rec.x, rec.y, rec.width, rec.height, RED);
  }
};

class Board : public Drawable {
private:
  Image img;
  Texture2D board;
  Texture2D queen;

  Rectangle board_src;
  float scale;
  Rectangle board_des;

  Rectangle queen_src;
  Rectangle queen_des;

public:
  Board(const Image &img) : img(img) {
    board = LoadTextureFromImage(img);
    queen = LoadTextureFromImage(img);

    // clang-format off
    board_src = {
      0.f,
      4.f,
      (float)board.width,
      (float)board.height - 319
    };

    scale = (float)WIDTH / (float)board.width;
    board_des = {
      0.f,
      0.f, 
      board_src.width * scale,
      board_src.height * scale
    };

    const float tileW = board.width / 8.f;
    const float tileH = board.height / 10.f;
    queen_src = {
      tileW * 4 + 4,
      board.height - tileH * 2 - 6,
      tileW - 1,
      tileH - 2
    };

    const float tileW_screen = board_des.width / 8.f;
    const float tileH_screen = board_des.height / 8.f;
    queen_des = {
      board_des.x + tileW_screen * 1,
      board_des.y + tileH_screen * 1,
      queen_src.width * scale,
      queen_src.height * scale
    };
    // clang-format on
  }

  ~Board() { UnloadImage(img); }

  void draw() const override {
    DrawTexturePro(board, board_src, board_des, {0, 0}, 0, WHITE);
  }

  const Rectangle &getBoardSrc() const { return board_src; }
  const Rectangle &getBoardDes() const { return board_des; }
  const Rectangle &getQueenSrc() const { return queen_src; }
  float getScale() const { return scale; }
  const Texture2D &getBoardTexture() const { return board; }
  const Texture2D &getQueenTexture() const { return queen; }
};

class Queen : public Drawable {
  int row, col;
  const Board &board;

public:
  Queen(int r, int c, const Board &b) : row(r), col(c), board(b) {}

  void draw() const override {
    float tileW = board.getBoardDes().width / 8.0f;
    float tileH = board.getBoardDes().height / 8.0f;

    // clang-format off
    Rectangle queen_des = {
      board.getBoardDes().x + col * tileW,
      board.getBoardDes().y + row * tileH,
      board.getQueenSrc().width * board.getScale(),
      board.getQueenSrc().height * board.getScale()
    };

    // clang-format on
  }
};

int main() {
  InitWindow(WIDTH, HEIGHT, "N-Queen");
  SetTargetFPS(60);

  Board board(LoadImage("../assets/Board.png")); // can't modify
  Button activate({700.f, 850.f}, 25.f, GREEN);
  Queen queen_1(0, 0, board);

  vector<Drawable *> drawables;
  drawables.push_back(&board);
  drawables.push_back(&queen_1);
  drawables.push_back(&activate);

  while (!WindowShouldClose()) {
    activate.update();

    // begin of drawing
    BeginDrawing();
    ClearBackground(WHITE);

    for (auto d : drawables)
      d->draw();
    // end of drawing
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
