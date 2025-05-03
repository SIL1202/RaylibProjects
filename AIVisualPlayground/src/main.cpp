#include "../include/raylib.h"
#include "Global.h"
#include <chrono>
#include <queue>
#include <thread>
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

void BFS(Graph &g) {
  std::vector<bool> visited(g.nodes.size(), false);
  std::queue<int> q;
  g.nodes[0].color = GREEN;
  visited[0] = true;
  q.push(0); // init node

  while (!q.empty()) {
    int current = q.front();
    q.pop();

    for (int neighbor : g.nodes[current].neighbors) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        g.nodes[neighbor].color = GREEN;

        BeginDrawing();
        ClearBackground(Hex_to_deci("ffffff"));
        g.draw();
        EndDrawing();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        q.push(neighbor);
      }
    }
  }
}

int main() {
  // initialize
  InitWindow(800, 600, "graph");

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

  BFS(G);
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
