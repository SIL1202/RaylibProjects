#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>

class Tile {
private:
  Rectangle rec;
  bool edge_exposing;
  int color;

public:
  Tile(Vector2 p, int c, bool ee) : color(c), edge_exposing(ee) {
    rec.x = p.x;
    rec.y = p.y;
    rec.width = GetScreenWidth() / 20.f;
    rec.height = GetScreenHeight() / 20.f;
  }

  Rectangle getRec() const { return rec; }
  bool is_edge_exposing() const { return edge_exposing; }
  void setEdge_exposing(bool v = false) { edge_exposing = v; }

  Color getC() const { return (color ? RAYWHITE : BLACK); }

  void toggleColor() { color = !color; }

  void setColorFromSil(Color c) {
    color = (c.r == 255 && c.g == 255 && c.b == 255) ? 1 : 0;
  }

  void update();
  void draw() const {
    DrawRectangle(rec.x, rec.y, rec.width, rec.height, getC());
  }
};

class Sil {
private:
  Vector2 position;
  Color color;
  float radius;
  Vector2 v_0;

public:
  Sil(Vector2 pos, Color c, float r, Vector2 v)
      : position(pos), color(c), radius(r), v_0(v) {}

  Vector2 getPos() const { return position; }
  float getR() const { return radius; }
  Color getC() const { return color; }

  void switchV(Vector2 collisionPoint) {
    Vector2 normal =
        Vector2Normalize(Vector2Subtract(position, collisionPoint));
    Vector2 reflected = Vector2Subtract(
        v_0, Vector2Scale(normal, 2 * Vector2DotProduct(v_0, normal)));
    v_0 = reflected;
  }

  void update(float dt) {
    position.x += v_0.x * dt;
    position.y += v_0.y * dt;
    if (position.x < 10 || position.x > 800 - 10)
      v_0.x *= -1;
    if (position.y < 10 || position.y > 800 - 10)
      v_0.y *= -1;

    std::cout << "dt = " << dt << '\n';
    std::cout << "x = " << position.x << '\n';
    std::cout << "y = " << position.y << '\n';
  }

  void draw() { DrawCircle(position.x, position.y, radius, color); }
};

int main(void) {
  InitWindow(800, 800, "destroy");
  SetTargetFPS(60);

  std::vector<std::vector<Tile>> top(20);
  std::vector<std::vector<Tile>> down(20);

  float tile_width = 800.f / 20.f;
  float tile_height = 800.f / 20.f;
  for (int i = 0; i < 20; ++i) {
    if (i < 10) {
      for (int j = 0; j < 20; ++j) {
        top[i].push_back(Tile({j * tile_width, i * tile_height}, 1,
                              (i == 9 ? true : false)));
      }
    } else {
      for (int j = 0; j < 20; ++j) {
        down[i].push_back(Tile({j * tile_width, i * tile_height}, 0,
                               (i == 10 ? true : false)));
      }
    }
  }

  Vector2 pos1 = {(float)GetRandomValue(10, 800 - 10),
                  (float)GetRandomValue(10, 400 - 10)};
  Vector2 pos2 = {(float)GetRandomValue(10, 800 - 10),
                  (float)GetRandomValue(400 + 10, 800 - 10)};

  Sil sil1(pos1, BLACK, 10.f, {0, 100});
  Sil sil2(pos2, RAYWHITE, 10.f, {0, -100});

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    BeginDrawing();

    sil1.update(dt);
    sil2.update(dt);

    ClearBackground(RAYWHITE);

    for (auto &row : top) {
      for (auto &tile : row) {
        if (tile.is_edge_exposing() &&
            CheckCollisionCircleRec(sil2.getPos(), sil2.getR(),
                                    tile.getRec())) {
          // 找出圓心與矩形之間最近的點（投影點）
          float closestX = std::max(
              sil2.getPos().x,
              std::min(tile.getRec().x, tile.getRec().x + tile.getRec().width));
          float closestY = std::max(
              sil2.getPos().y,
              std::min(tile.getRec().y, tile.getRec().y + tile.getRec().width));

          // collision point
          Vector2 collisionPoint = {closestX, closestY};
          tile.setEdge_exposing();
          tile.setColorFromSil(sil2.getC());
          sil2.switchV(collisionPoint);
        }
        tile.draw();
      }
    }

    for (auto &row : down) {
      for (auto &tile : row) {
        if (tile.is_edge_exposing() &&
            CheckCollisionCircleRec(sil1.getPos(), sil1.getR(),
                                    tile.getRec())) {
          float closestX = std::max(
              sil1.getPos().x,
              std::min(tile.getRec().x, tile.getRec().x + tile.getRec().width));
          float closestY = std::max(
              sil1.getPos().y,
              std::min(tile.getRec().y, tile.getRec().y + tile.getRec().width));

          Vector2 collisionPoint = {closestX, closestY};
          tile.setEdge_exposing();
          tile.setColorFromSil(sil1.getC());
          sil1.switchV(collisionPoint);
        }
        tile.draw();
      }
    }

    sil1.draw();
    sil2.draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
