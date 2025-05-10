#include "../include/raylib.h"
#include "Global.h"
#include <iostream>
#include <queue>
#include <vector>

Font font;
struct Vertex {
  int centerX, centerY;
  float r;
  Color color;
  const char *text;
  std::vector<int> neighbors;

  void draw() const {
    DrawCircle(centerX, centerY, r, color);
    int textWidth = MeasureText(text, 20);
    DrawTextEx(font, text,
               {(float)(centerX - textWidth) / 2, (float)centerY - 10}, 20, 5,
               WHITE);
  }
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
      node.draw();
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

  void reset() {
    for (auto g : G->nodes) {
      g.color = BLACK;
    }
  }
};

int main() {
  // initialize
  InitWindow(800, 600, "graph");
  font = LoadFontEx("../assets/JetBrainsMonoNerdFont-Bold.ttf", 20, 0, 250);
  Botton Next("../assets/triangle.png",
              {(float)GetScreenWidth() - Margin.right * 8,
               (float)GetScreenHeight() - 65});

  Botton Reset({(float)GetScreenWidth() - Margin.right * 4,
                (float)GetScreenHeight() - 60, 40.f, 40.f});
  Graph G;
  // A = 0
  G.nodes.push_back({150, 150, 20, BLACK, "A", {2, 4}}); // A -> C, E
  // B = 1
  G.nodes.push_back({650, 150, 20, BLACK, "B", {5}}); // B -> F
  // C = 2
  G.nodes.push_back({350, 180, 20, BLACK, "C", {1, 3, 5}}); // C -> B, D, F
                                                            // D = 3
  G.nodes.push_back({350, 400, 20, BLACK, "D", {5}});       // D -> F
  // E = 4
  G.nodes.push_back({120, 500, 20, BLACK, "E", {5}}); // E -> F
  // F = 5
  G.nodes.push_back({650, 450, 20, BLACK, "F", {}}); // F has no outgoing edges

  Block B;
  BFSrunner bfs(&G);
  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();
    ClearBackground(Hex_to_deci("ffffff"));
    B.draw();
    G.draw();
    Next.draw_tri();
    Reset.draw_rec();
    if (Next.isPressed_tri())
      bfs.step();
    if (Reset.isPressed_rec())
      bfs.reset();
    // end drawing
    EndDrawing();
  }

  // close window
  CloseWindow();
}
