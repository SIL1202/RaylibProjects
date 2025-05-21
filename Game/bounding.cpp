#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <cstdlib>

#define WIDTH 800
#define HEIGHT 600

class Ball {
public:
  Ball() {}
  Ball(Vector2 p, float r, Color c) : pos(p), r(r), color(c), velocity(0.f) {}

  void draw() const { DrawCircle(pos.x, pos.y, r, color); }

  static void connect(const Ball &b1, const Ball &b2) {
    Vector2 dir = Vector2Subtract(b2.pos, b1.pos);
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0)
      return;

    Vector2 unitDir = {dir.x / length, dir.y / length};

    Vector2 newS = {b1.pos.x + unitDir.x * b1.r, b1.pos.y + unitDir.y * b1.r};
    Vector2 newE = {b2.pos.x - unitDir.x * b2.r, b2.pos.y - unitDir.y * b2.r};

    DrawLineEx(newS, newE, 10.f, RED);
  }

private:
  Vector2 pos;
  float r;
  Color color;
  float velocity;
};

int main() {

  InitWindow(WIDTH, HEIGHT, "BOUNDING");
  Ball ball1({200, 400}, 50, BLACK);
  Ball ball2({500, 200}, 50, BLACK);
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(WHITE);

    ball1.draw();
    ball2.draw();
    Ball::connect(ball1, ball2);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
