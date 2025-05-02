#include "../include/raylib.h"
#include "Global.h"
#include <vector>

struct Vertex {
  int centerX, centerY;
  float r;
  Color color;
  void draw() const { DrawCircle(centerX, centerY, r, color); }
};

struct Edge {
  int startPosX, startPosY, endPosX, endPosY;
  Color color;
  void draw() const { DrawLine(startPosX, startPosY, endPosX, endPosY, color); }
};

struct Graph {
  std::vector<std::pair<Vertex, Vertex>> G;

  void draw() const {
    for (const auto &g : G) {
      g.first.draw();
      g.second.draw();
    }
  }
};

int main() {
  // initialize
  InitWindow(800, 600, "graph");
  Vertex v1({300, 200, 25.f, Hex_to_deci("000000")});
  Vertex v2({500, 400, 25.f, Hex_to_deci("000000")});

  Graph G = {{{v1, v2}}};
  // update
  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();
    ClearBackground(Hex_to_deci("ffffff"));
    G.draw();
    // end drawing
    EndDrawing();
  }

  // close window
  CloseWindow();
}
