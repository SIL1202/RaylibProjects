#include "main.h"
#define PLAYER_CNT 2

static void pvp_init();
static void pvp_update();
static void pvp_draw();

GameState gamestate = STATE_MENU;

// player structure
typedef struct Player {
  Rectangle rec;
  Color c;
} Player;

// player declaration in array
static Player player[PLAYER_CNT];

// main function
int main(void) {
  InitWindow(800, 800, "pingpong");
  menu_init();
  pvp_init();
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    if (gamestate == STATE_MENU) {
      menu_update();
      menu_draw();
    } else if (gamestate == STATE_PVP) {
      pvp_update();
      pvp_draw();
    }
    // else {}

    EndDrawing();
  }
  CloseWindow();
  return 0;
}

// PVP function definitions
static void pvp_init() {
  // init player objects
  player[0] = (Player){{10, 10, 10, 100}, RAYWHITE};
  player[1] = (Player){{780, 10, 10, 100}, RAYWHITE};
}
static void pvp_update() {}
static void pvp_draw() {
  for (int i = 0; i < PLAYER_CNT; ++i)
    DrawRectangleRec(player[i].rec, player[i].c);
}
