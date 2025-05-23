#include "raylib.h"
#include <cmath>
#include <iostream>
#include <numeric>
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

class Button {
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
  void draw() {
    if (game::state == game::State::end)
      DrawCircle(vec.x, vec.y, rad, col);
    else
      DrawRectangle(rec.x, rec.y, rec.width, rec.height, RED);
  }
};

class N_Queen {
public:
  void printBoard(const vector<int> &board, int n) {
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        if (board[row] == col) {
          cout << " Q ";
        } else {
          cout << " . ";
        }
      }
      cout << '\n';
    }
  }

  bool isValid(const vector<int> &board, int row, int col) {
    for (int i = 0; i < row; i++) {
      if (board[i] == col || abs(board[i] - col) == abs(i - row))
        return false;
    }
    return true;
  }

  bool solve(int row, vector<int> &board, int n) {
    if (row == n) {
      printBoard(board, n); // find solution
      cout << '\n';
    }

    for (int col = 0; col < n; col++) {
      if (isValid(board, row, col)) {
        board[row] = col;
        solve(row + 1, board, n);
        board[row] = -1; // backtracking
      }
    }

    return false; // 無法在這一層找到合法解
  }
};

int main() {
  InitWindow(WIDTH, HEIGHT, "N-Queen");
  SetTargetFPS(60);
  Image img = LoadImage("../assets/Board.png");
  Texture2D background = LoadTextureFromImage(img);
  UnloadImage(img);

  Rectangle src = {0.f, 4.f, (float)background.width,
                   (float)background.height - 319};
  float scale = (float)WIDTH / (float)src.width;
  Rectangle des = {0.f, 0.f, src.width * scale, src.height * scale};

  Button activate({700.f, 850.f}, 25.f, GREEN);
  while (!WindowShouldClose()) {
    activate.update();

    BeginDrawing();
    ClearBackground(WHITE);

    DrawTexturePro(background, src, des, {0.f, 0.f}, 0.f, WHITE);
    activate.draw();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
/*
int main() {
  int n;
  cout << "Enter the size of grid: ";
  cin >> n;

  vector<int> board(n, -1);

  if (!solve(0, board, n)) {
    cout << "No solution exists for n = " << n << endl;
  }

  return 0;
} */

// {1, 3, 0, 2}
// . Q . .
// . . . Q
// Q . . .
// . . Q .

//
//     0   1   2   3   4
//   +------------------
// 0 | \               /
// 1 |     \       /
// 2 |         Q
// 3 |     /       \
// 4 | /               \
