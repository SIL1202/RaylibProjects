#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Ball's structure
typedef struct Ball {
  Vector2 pos;
  float r;
  int c;

  Vector2 vel;
} Ball;

// Box's structure
typedef struct Box {
  Vector2 pos;
  float w, h;
  int c; // 1(DARKGRAY), -1(LIGHTGRAY)
} Box;

// Tile's structure
typedef struct Tile {
  size_t r, c;
  Box **tiles;
} Tile;

// box's objects
void box_update(Box *box) {}
void box_draw(const Box *box) {
  // draw rectangle
  Rectangle b = {box->pos.x, box->pos.y, box->w, box->h};
  DrawRectangleRec(b, BLACK);
}

Ball *init_ball(Vector2 pos, int c) {
  Ball *b = malloc(sizeof(Ball));
  Vector2 center = pos;
  float angle = (float)GetRandomValue(0, 360);
  float speed = 1000.f;
  Vector2 velocity = {cosf(angle) * speed, sinf(angle) * speed};
  *b = (Ball){center, 25.f, c, velocity};
  return b;
}

// ball's objects
void ball_update(Ball *ball, const Box *box, float dt) {
  // update position
  ball->pos.x += ball->vel.x * dt;
  ball->pos.y += ball->vel.y * dt;

  Rectangle box_dummy = {box->pos.x, box->pos.y, box->w, box->h};

  // check collision
  /*
  if (CheckCollisionCircleRec(ball->pos, ball->r, box_dummy)) {
    float closestX = MAX(box->pos.x, MIN(ball->pos.x, box->pos.x + box->w));
    float closestY = MAX(box->pos.y, MIN(ball->pos.y, box->pos.y + box->h));
    Vector2 collisionPoint = {closestX, closestY};
    Vector2 normal =
        Vector2Normalize(Vector2Subtract(ball->pos, collisionPoint));
    Vector2 reflected = Vector2Subtract(
        ball->vel,
        Vector2Scale(normal, 2 * Vector2DotProduct(ball->vel, normal)));
    ball->vel = reflected;

    ball->pos = Vector2Add(ball->pos, Vector2Scale(normal, 0.1f));
  }
  */

  // check if out of bounds
  if (ball->pos.x - ball->r < 0) {
    ball->pos.x = ball->r;
    ball->vel.x *= -1;
  } else if (ball->pos.x + ball->r > GetScreenWidth()) {
    ball->pos.x = GetScreenWidth() - ball->r;
    ball->vel.x *= -1;
  }

  if (ball->pos.y - ball->r < 0) {
    ball->pos.y = ball->r;
    ball->vel.y *= -1;
  } else if (ball->pos.y + ball->r > GetScreenHeight()) {
    ball->pos.y = GetScreenHeight() - ball->r;
    ball->vel.y *= -1;
  }
}

// drawing ball in circle
void ball_draw(const Ball *ball) {
  DrawCircleV(ball->pos, ball->r, (ball->c > 0 ? DARKGRAY : LIGHTGRAY));
}

// init each part of tiles
Tile *init_tiles() {
  const float w = GetScreenWidth() / 20.f;
  const float h = GetScreenHeight() / 20.f;
  const size_t row = 20, col = 20;
  Tile *tile = malloc(sizeof(Tile));
  tile->r = row, tile->c = col;

  tile->tiles = malloc(sizeof(Box *) * row);
  for (int i = 0; i < row; ++i) {
    tile->tiles[i] = malloc(sizeof(Box) * col);
    int color = (i < row / 2) ? -1 : 1;
    for (int j = 0; j < col; ++j)
      tile->tiles[i][j] = (Box){{j * w, i * h}, w, h, color};
  }
  return tile;
}

void tiles_update(Tile *part, Ball *ball, const bool isTop) {
  for (int i = 0; i < part->r; ++i) {
    for (int j = 0; j < part->c; ++j) {
      // clang-format off
      Rectangle rec = {
        part->tiles[i][j].pos.x,
        part->tiles[i][j].pos.y,
        part->tiles[i][j].w,
        part->tiles[i][j].h
      };

      if (CheckCollisionCircleRec(ball->pos, ball->r, rec)) {
        if ((ball->c == 1 && part->tiles[i][j].c > 0) ||
            (ball->c == -1 && part->tiles[i][j].c < 0)) {
          float closestX = MAX(rec.x, MIN(ball->pos.x, rec.x + rec.width));
          float closestY = MAX(rec.y, MIN(ball->pos.y, rec.y + rec.height));
          Vector2 collisionPoint = {closestX, closestY};
          Vector2 normal = Vector2Normalize(Vector2Subtract(ball->pos, collisionPoint));
          Vector2 reflected = Vector2Subtract(ball->vel,
            Vector2Scale(normal, 2 * Vector2DotProduct(ball->vel, normal)));
          ball->vel = reflected;
          ball->pos = Vector2Add(ball->pos, Vector2Scale(normal, 0.1f));
          part->tiles[i][j].c *= -1;
        }
      }
      // clang-format on
    }
  }
}

// drawing each part of tiles
void tiles_draw(const Tile *part) {
  for (int i = 0; i < part->r; ++i)
    for (int j = 0; j < part->c; ++j) {
      Rectangle rec = {part->tiles[i][j].pos.x, part->tiles[i][j].pos.y,
                       part->tiles[i][j].w, part->tiles[i][j].h};
      Color c = part->tiles[i][j].c > 0 ? DARKGRAY : LIGHTGRAY;
      DrawRectangleRec(rec, c);
    }
}

void tiles_free(Tile *tile) {
  for (int i = 0; i < tile->r; ++i) {
    free(tile->tiles[i]);
  }
  free(tile->tiles);
  free(tile);
}

int main(void) {
  InitWindow(800, 800, "Box");
  // init box
  Box *box = (Box *)malloc(sizeof(Box));
  *box = (Box){{350, 350}, 100, 100};

  // init ball1
  Ball *b1 =
      init_ball((Vector2){GetRandomValue(50, 750), GetRandomValue(50, 350)}, 1);
  // init ball2
  Ball *b2 = init_ball(
      (Vector2){GetRandomValue(50, 750), GetRandomValue(450, 750)}, -1);

  // init top part and down part tiles

  Tile *tiles = init_tiles();

  // game loop
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // game updating
    ball_update(b1, box, dt);
    ball_update(b2, box, dt);
    tiles_update(tiles, b1, false); // down, b1
    tiles_update(tiles, b2, true);  // top, b2
    // game drawing
    tiles_draw(tiles);
    // box_draw(box);
    ball_draw(b1);
    ball_draw(b2);

    EndDrawing();
  }
  free(box);
  free(b1);
  free(b2);
  tiles_free(tiles);
  CloseWindow();
  return 0;
}
