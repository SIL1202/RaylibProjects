#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <cstdlib>

#define WIDTH 800
#define HEIGHT 600

class Ball {
public:
  Ball() {}
  Ball(Vector2 p, float r, Color c) : pos(p), r(r), color(c) {
    float angle = GetRandomValue(0, 360) * DEG2RAD;
    float speed = GetRandomValue(100, 300);
    v = {cosf(angle) * speed, sinf(angle) * speed};
  }

  static void connect(const Ball &b1, const Ball &b2) {
    Vector2 dir = Vector2Subtract(b2.pos, b1.pos);
    float length = Vector2Length(dir);
    if (length == 0)
      return;

    Vector2 unitDir = {dir.x / length, dir.y / length};

    Vector2 newS = {b1.pos.x + unitDir.x * b1.r, b1.pos.y + unitDir.y * b1.r};
    Vector2 newE = {b2.pos.x - unitDir.x * b2.r, b2.pos.y - unitDir.y * b2.r};

    float thickness = 3000.f / length;
    thickness = Clamp(thickness, 1.f, 20.f);
    DrawLineEx(newS, newE, thickness, RED);
  }

  void update(float dt) {
    pos.x += v.x * dt;
    pos.y += v.y * dt;

    if (pos.x - r <= 0 || pos.x + r >= WIDTH)
      v.x *= -1;

    if (pos.y - r <= 0 || pos.y + r >= HEIGHT)
      v.y *= -1;
  }

  void draw() const { DrawCircle(pos.x, pos.y, r, color); }

private:
  Vector2 pos;
  float r;
  Color color;
  Vector2 v;
};

int main() {

  InitWindow(WIDTH, HEIGHT, "BOUNCING");
  SetTargetFPS(60);

  Ball ball1({200, 400}, 50, BLACK);
  Ball ball2({500, 200}, 50, BLACK);
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    BeginDrawing();
    ball1.update(dt);
    ball2.update(dt);

    ClearBackground(WHITE);

    ball1.draw();
    ball2.draw();
    Ball::connect(ball1, ball2);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
