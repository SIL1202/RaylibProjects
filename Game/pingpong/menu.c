#include "main.h"
#define MENU_ITEM_CNT 2

typedef struct {
  Rectangle rec;
  Vector2 pos;
  const char *label;
  Color rec_color;
  GameState next_state;
} MenuItem;

static MenuItem menuItems[MENU_ITEM_CNT];

void menu_init() {
  menuItems[0] =
      (MenuItem){{10, 290, 780, 60}, {370, 300}, "PVP", BLACK, STATE_PVP};
  menuItems[1] =
      (MenuItem){{10, 490, 780, 60}, {370, 500}, "PVC", BLACK, STATE_PVC};
}

bool isHover(Rectangle rec) {
  return CheckCollisionPointRec(GetMousePosition(), rec);
}

bool isClick(Rectangle rec) {
  return CheckCollisionPointRec(GetMousePosition(), rec) &&
         IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void menu_update() {
  for (int i = 0; i < MENU_ITEM_CNT; ++i) {
    menuItems[i].rec_color = BLACK;
    if (isHover(menuItems[i].rec))
      menuItems[i].rec_color = LIGHTGRAY;
    if (isClick(menuItems[i].rec))
      gamestate = menuItems[i].next_state;
  }
}

void menu_draw() {
  for (int i = 0; i < MENU_ITEM_CNT; ++i) {
    DrawRectangleRec(menuItems[i].rec, menuItems[i].rec_color);
    DrawText(menuItems[i].label, menuItems[i].pos.x, menuItems[i].pos.y, 40,
             RAYWHITE);
  }
}
