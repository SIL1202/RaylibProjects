#include "../include/raylib.h"
#include "Global.h"
#include <iostream>
#include <queue>
#include <vector>

struct Vertex {
  int centerX, centerY;
  float r;
  Color color;
  std::vector<int> neighbors;

  void draw() const { DrawCircle(centerX, centerY, r, color); }
};

struct Graph {
  std::vector<Vertex> nodes;

  void draw() const {
    for (const Vertex &node : nodes) {
      for (int i : node.neighbors) {
        DrawLine(node.centerX, node.centerY, nodes[i].centerX, nodes[i].centerY,
                 BLACK);
      }
    }
    for (const Vertex &node : nodes)
      DrawCircle(node.centerX, node.centerY, node.r, node.color);
  }
};

class Block {
private:
  std::vector<std::pair<Rectangle, const char *>> blocks;

public:
  Block() {
    float startX = 50;
    float startY = GetScreenHeight() - 60;
    for (int i = 0; i < 6; i++) {
      blocks.push_back({{startX + i * 40, startY, 40, 40}, ""});
    }
  }
  void enqueue(const char *text, int index) {
    if (index < blocks.size())
      blocks[index].second = text;
  }
  void dequeue(int index) {
    if (index < blocks.size())
      blocks[index].second = "";
  }
  void draw() {
    for (auto &block : blocks) {
      DrawRectangleRec(block.first, LIGHTGRAY);
      DrawRectangleLinesEx(block.first, 2, BLACK);
      if (block.second) {
        float textWidth = MeasureText(block.second, 20);
        DrawText(block.second,
                 block.first.x + block.first.width / 2 - textWidth / 2,
                 block.first.y + block.first.height / 2 - 10, 20, BLACK);
      }
    }
  }
};

class BFSrunner {
public:
  Graph *G;
  std::vector<bool> visited;
  std::queue<int> q;

  BFSrunner(Graph *g) : G(g), visited(g->nodes.size(), false) {
    visited[0] = true;
    q.push(0); // init node
    G->nodes[0].color = GREEN;
  }

  bool step() {
    if (q.empty())
      return false;
    int current = q.front();
    q.pop();

    for (int neighbor : G->nodes[current].neighbors) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        G->nodes[neighbor].color = GREEN;
        q.push(neighbor);
        break;
      }
    }
    return true;
  }
};

int main() {
  // initialize
  InitWindow(800, 600, "graph");

  Botton Next("../assets/triangle.png",
              {(float)GetScreenWidth() - Margin.right * 6,
               (float)GetScreenHeight() - 65});

  Graph G;
  // A = 0
  G.nodes.push_back({150, 150, 20, BLACK, {2, 4}}); // A -> C, E
  // B = 1
  G.nodes.push_back({650, 150, 20, BLACK, {5}}); // B -> F
  // C = 2
  G.nodes.push_back({350, 180, 20, BLACK, {1, 3, 5}}); // C -> B, D, F
  // D = 3
  G.nodes.push_back({350, 400, 20, BLACK, {5}}); // D -> F
  // E = 4
  G.nodes.push_back({120, 500, 20, BLACK, {5}}); // E -> F
  // F = 5
  G.nodes.push_back({650, 450, 20, BLACK, {}}); // F has no outgoing edges

  Block B;
  BFSrunner bfs(&G);
  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();
    ClearBackground(Hex_to_deci("ffffff"));
    B.draw();
    G.draw();
    Next.draw();
    // bfs.step();
    // end drawing
    EndDrawing();
  }

  // close window
  CloseWindow();
}
