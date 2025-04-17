#include "include/raylib.h"

typedef struct Sprite {
  Texture2D Texture;
  Rectangle dest_rect;
} Sprite;

void move_sprite(Sprite *sprite) {
  if (IsKeyDown(KEY_D)) {
    sprite->dest_rect.x += 500.0 * GetFrameTime();
  }
  if (IsKeyDown(KEY_A)) {
    sprite->dest_rect.x -= 500.0 * GetFrameTime();
  }
  if (IsKeyDown(KEY_W)) {
    sprite->dest_rect.y -= 500.0 * GetFrameTime();
  }
  if (IsKeyDown(KEY_S)) {
    sprite->dest_rect.y += 500.0 * GetFrameTime();
  }
}

int main() {
  // init app

  InitWindow(600, 400, "SIL");

  Image img = LoadImage("assets/sil.png");
  Texture2D player_sil = LoadTextureFromImage(img);
  UnloadImage(img);

  Sprite player = (Sprite){.Texture = player_sil,
                           .dest_rect = (Rectangle){
                               .x = 0.0,
                               .y = 0.0,
                               .width = player_sil.width * 0.125f,
                               .height = player_sil.height * 0.125f,
                           }};

  // run app
  while (!WindowShouldClose()) {
    // update section
    move_sprite(&player);

    BeginDrawing();

    // add drawing happens
    ClearBackground(WHITE);

    DrawTexturePro(
        player.Texture,
        {0, 0, (float)player.Texture.width, (float)player.Texture.height},
        player.dest_rect, {0, 0}, 0.0, RAYWHITE);
    EndDrawing();
  }

  UnloadTexture(player_sil);
  // close app

  CloseWindow();
}
