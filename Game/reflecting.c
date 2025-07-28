#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Box's structure
typedef struct Box {
  Vector2 pos;
  float w, h;
  Color c;
} Box;

// Ball's structure
typedef struct Ball {
  Vector2 pos;
  float r;
  Color c;

  Vector2 vel;
} Ball;

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

// ball's objects
void ball_update(Ball *ball, const Box *box, float dt) {
  // update position
  ball->pos.x += ball->vel.x * dt;
  ball->pos.y += ball->vel.y * dt;

  Rectangle box_dummy = {box->pos.x, box->pos.y, box->w, box->h};
  // check collision
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
void ball_draw(const Ball *ball) { DrawCircleV(ball->pos, ball->r, ball->c); }

// init each part of tiles
Tile *init_tiles(Vector2 startPos, float w, float h, Color c) {
  size_t row = 10, col = 20; // each has 10 rows and 20 cols
  Tile *tile = malloc(sizeof(Tile));
  tile->r = row, tile->c = col;

  tile->tiles = malloc(sizeof(Box *) * row);
  for (int i = 0; i < row; ++i) {
    tile->tiles[i] = malloc(sizeof(Box) * col); // number in single row
    for (int j = 0; j < col; ++j)
      tile->tiles[i][j] = (Box){{j * w, startPos.y + i * h}, w, h, c};
  }
  return tile;
}

// drawing each part of tiles
void tiles_draw(const Tile *part) {
  for (int i = 0; i < part->r; ++i)
    for (int j = 0; j < part->c; ++j) {
      Rectangle rec = {part->tiles[i][j].pos.x, part->tiles[i][j].pos.y,
                       part->tiles[i][j].w, part->tiles[i][j].h};
      Color c = part->tiles[i][j].c;
      DrawRectangleRec(rec, c);
    }
}

void tiles_free(Tile *tile) {
  for (int i = 0; i < tile->r; ++i) {
    free(tile->tiles[i]); // 釋放每一行
  }
  free(tile->tiles); // 釋放 row 指標陣列
  free(tile);        // 最後釋放 Tile 結構本身
}

int main(void) {
  InitWindow(800, 800, "Box");
  // init box
  Box *box = (Box *)malloc(sizeof(Box));
  *box = (Box){{350, 350}, 100, 100};

  // init ball1
  Ball *b1 = (Ball *)malloc(sizeof(Ball));
  Vector2 center =
      (Vector2){(float)GetRandomValue(50, 750), (float)GetRandomValue(50, 750)};
  float angle = (float)GetRandomValue(0, 360);
  float speed = 2000.f;
  Vector2 velocity = {cosf(angle) * speed, sinf(angle) * speed};
  *b1 = (Ball){center, 25.f, BLACK, velocity};

  // init ball2
  Ball *b2 = (Ball *)malloc(sizeof(Ball));
  center =
      (Vector2){(float)GetRandomValue(50, 750), (float)GetRandomValue(50, 750)};
  angle = (float)GetRandomValue(0, 360);
  velocity = (Vector2){cosf(angle) * speed, sinf(angle) * speed};
  *b2 = (Ball){center, 25.f, WHITE, velocity};

  // init top part and down part tiles
  float box_width = GetScreenWidth() / 20.f;
  float box_height = GetScreenHeight() / 20.f;
  Tile *top = init_tiles((Vector2){0.f, 0.f}, box_width, box_height, LIGHTGRAY);
  Tile *down =
      init_tiles((Vector2){0.f, 400.f}, box_width, box_height, DARKGRAY);

  // game loop
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // game updating
    ball_update(b1, box, dt);
    ball_update(b2, box, dt);

    // game drawing
    tiles_draw(top);
    tiles_draw(down);
    box_draw(box);
    ball_draw(b1);
    ball_draw(b2);

    EndDrawing();
  }
  free(box);
  free(b1);
  free(b2);
  tiles_free(top);
  tiles_free(down);
  CloseWindow();
  return 0;
}
