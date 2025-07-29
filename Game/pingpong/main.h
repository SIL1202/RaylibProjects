#define MAIN_H
#ifdef MAIN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>

typedef enum { STATE_MENU, STATE_PVP, STATE_PVC } GameState;
extern GameState gamestate;

void menu_init();
void menu_update();
void menu_draw();

#endif // MAIN_H
