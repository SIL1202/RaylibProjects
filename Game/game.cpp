#include <raylib.h>
#include <vector>

// For all characters Width and Height
float Width, Height;

enum SpriteDirection { Left = -1, Right = 1 };

// clang-format off
const int level[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 
  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
  0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0,
  0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
// clang-format on

typedef struct Sprite {
  Texture2D Texture;            // store entire picture
  Rectangle dest_rect;          // cut a part of picture
  Vector2 velocity{0.0f, 0.0f}; // velocity value
  SpriteDirection dir;
} Sprite;

typedef Sprite SpriteTile; // Sprite for tiles

void move_player(Sprite *sprite) {
  sprite->velocity.x = 0.0f;
  if (IsKeyDown(KEY_A)) {
    sprite->velocity.x = -200.0f;
    sprite->dir = SpriteDirection::Left;
  }
  if (IsKeyDown(KEY_D)) {
    sprite->velocity.x = 200.0f;
    sprite->dir = SpriteDirection::Right;
  }
  if (IsKeyPressed(KEY_SPACE)) {
    sprite->velocity.y = -400.0f;
  }
}

void apply_gravity(Sprite *sprite) {
  float gravity = 2000.0f;
  // v = v + at
  sprite->velocity.y += gravity * GetFrameTime();
}

void apply_vel_y(Sprite *sprite) {
  // y = y + vt
  sprite->dest_rect.y += sprite->velocity.y * GetFrameTime();
}

void apply_vel_x(Sprite *sprite) {
  sprite->dest_rect.x += sprite->velocity.x * GetFrameTime();
}

void checkCollision_y(Sprite *sprite, std::vector<SpriteTile> &tiles) {
  for (const auto &tile : tiles) {
    if (CheckCollisionRecs(sprite->dest_rect, tile.dest_rect)) {
      float spriteBottom = sprite->dest_rect.y + sprite->dest_rect.height;
      float spriteTop = sprite->dest_rect.y;
      float tileTop = tile.dest_rect.y;
      float tileBottom = tile.dest_rect.y + tile.dest_rect.height;

      if (sprite->velocity.y > 0 && spriteBottom <= tileTop + 10) {

        sprite->dest_rect.y = tileTop - sprite->dest_rect.height;
        sprite->velocity.y = 0.0f;
      } else if (sprite->velocity.y < 0 && spriteTop >= tileBottom - 10) {

        sprite->dest_rect.y = tileBottom;
        sprite->velocity.y = 0.0f;
      }
    }
  }
}

void checkCollision_x(Sprite *sprite, std::vector<SpriteTile> &tiles) {
  for (const auto &tile : tiles) {
    if (CheckCollisionRecs(sprite->dest_rect, tile.dest_rect)) {

      float spriteRight = sprite->dest_rect.x + sprite->dest_rect.width;
      float spriteLeft = sprite->dest_rect.x;
      float tileRight = tile.dest_rect.x + tile.dest_rect.width;
      float tileLeft = tile.dest_rect.x;

      if (sprite->velocity.x > 0) {
        // 往右撞牆
        sprite->dest_rect.x = tileLeft - sprite->dest_rect.width;
        sprite->velocity.x = 0.0f;
      } else if (sprite->velocity.x < 0) {
        // 往左撞牆
        sprite->dest_rect.x = tileRight;
        sprite->velocity.x = 0.0f;
      }
    }
  }
}

std::vector<SpriteTile> load_tiles(Texture2D tileTexture) {
  std::vector<SpriteTile> arr;
  const int tileCols = 12; // 一列顯示幾個
  const int tileSize = 50; // 每格的寬高

  for (size_t i = 0; i < sizeof(level) / sizeof(level[0]); i++) {
    if (level[i] > 0) {
      int x = (i % tileCols) * tileSize;
      int y = (i / tileCols) * tileSize;

      arr.push_back(SpriteTile{
          .Texture = tileTexture,
          .dest_rect =
              Rectangle{(float)x, (float)y, (float)tileSize, (float)tileSize},
      });
    }
  }

  return arr;
}
int main() {
  // init app

  InitWindow(600, 400, "SIL");

  Image img = LoadImage("assets/sil.png");
  Texture2D player_sil = LoadTextureFromImage(img);
  UnloadImage(img);
  Width = player_sil.width * 0.0525;
  Height = player_sil.height * 0.0525;

  Sprite player = (Sprite){.Texture = player_sil,
                           .dest_rect =
                               (Rectangle){
                                   .x = 0.0f,
                                   .y = -200.0f,
                                   .width = Width,
                                   .height = Height,
                               },
                           .dir = SpriteDirection::Right};

  Texture2D Tile = LoadTexture("assets/Tile.png");
  std::vector<SpriteTile> obstacle_tile = load_tiles(Tile);

  // run app
  while (!WindowShouldClose()) {
    // update section
    move_player(&player);

    // apply gravity
    apply_gravity(&player);

    // apply vel and check collision
    apply_vel_y(&player);
    checkCollision_y(&player, obstacle_tile);

    apply_vel_x(&player);
    checkCollision_x(&player, obstacle_tile);

    // Set SetGround
    if (player.dest_rect.y >= GetScreenHeight() - player.dest_rect.height) {
      player.dest_rect.y = GetScreenHeight() - player.dest_rect.height;
    }

    BeginDrawing();

    // add drawing happens
    ClearBackground(WHITE);

    for (const auto &obstacle : obstacle_tile) {
      DrawTextureEx(obstacle.Texture,
                    (Vector2){obstacle.dest_rect.x, obstacle.dest_rect.y}, 0.0f,
                    obstacle.dest_rect.width / (float)obstacle.Texture.width,
                    RAYWHITE);
    }
    DrawTexturePro(player.Texture,
                   {0.0f, 0.0f, (float)player.Texture.width * (float)player.dir,
                    (float)player.Texture.height},
                   player.dest_rect, {0.0f, 0.0f}, 0.0f, RAYWHITE);
    EndDrawing();
  }

  UnloadTexture(player_sil);
  UnloadTexture(Tile);
  // close app

  CloseWindow();
}
