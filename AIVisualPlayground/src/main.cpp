#include "../include/raylib.h"
#include "../include/raymath.h"
#include "Global.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

void DrawArrow(Vector2 start, Vector2 end, float arrowSize, Color color);

struct Vertex {
  int centerX, centerY;
  float r;
  Color color;
  const char *text;
  std::vector<int> neighbors;

  void draw() const {
    DrawCircle(centerX, centerY, r, color);
    int textWidth = MeasureText(text, 20);
    DrawText(text, (centerX - (float)textWidth / 2), (float)centerY - 10, 20,
             WHITE);
  }
};

struct Graph {
  std::vector<Vertex> nodes;

  void draw() const {
    for (const Vertex &node : nodes) {
      for (int i : node.neighbors) {
        Vector2 from = {(float)node.centerX, (float)node.centerY};
        Vector2 to = {(float)nodes[i].centerX, (float)nodes[i].centerY};

        // 拉回箭頭尾端
        float radius = nodes[i].r;
        Vector2 dir = Vector2Subtract(to, from);
        float len = Vector2Length(dir);
        if (len > 0.01f) {
          Vector2 unit = Vector2Scale(dir, 1.0f / len);
          to = Vector2Add(from, Vector2Scale(unit, len - radius));
        }

        DrawArrow(from, to, 10.0f, BLACK);
      }
    } /*  for (const Vertex &node : nodes) {
        for (int i : node.neighbors) {
          DrawLine(node.centerX, node.centerY, nodes[i].centerX,
      nodes[i].centerY, BLACK);
        }
      } */
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

void DrawArrow(Vector2 start, Vector2 end, float arrowSize, Color color) {
  // 先畫線條
  DrawLineEx(start, end, 1.5f, color);

  Vector2 dir = Vector2Subtract(end, start);
  float length = Vector2Length(dir);
  if (length < 0.001f)
    return; // 避免除以零或極短線條

  Vector2 unit = Vector2Scale(dir, 1.0f / length); // 歸一化
  Vector2 normal = {-unit.y, unit.x};              // 垂直向量

  // 限制箭頭大小不超過線條長度的 40%
  float actualArrowSize =
      (length * 0.4f < arrowSize) ? length * 0.4f : arrowSize;

  Vector2 p1 = end; // 箭頭尖端
  Vector2 p2 =
      Vector2Add(end, Vector2Scale(unit, -actualArrowSize)); // 箭頭基部
  Vector2 p3 =
      Vector2Add(p2, Vector2Scale(normal, actualArrowSize * 0.5f)); // 左側點
  Vector2 p4 =
      Vector2Add(p2, Vector2Scale(normal, -actualArrowSize * 0.5f)); // 右側點

  // 改用 DrawLine 畫箭頭（3 條線組成三角形）
  DrawLineEx(p1, p3, 2.5f, color);
  DrawLineEx(p1, p4, 2.5f, color);
  DrawLineEx(p3, p4, 2.5f, color);
}

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

  int currentNode = -1;
  size_t neighborIndex = 0;
  bool step() {
    if (currentNode == -1) {
      if (q.empty())
        return false;
      currentNode = q.front();
      q.pop();
      neighborIndex = 0;
    }

    auto neighbors = G->nodes[currentNode].neighbors;
    while (neighborIndex < neighbors.size()) {
      int neighbor = neighbors[neighborIndex++];
      if (!visited[neighbor]) {
        G->nodes[neighbor].color = GREEN;
        q.push(neighbor);
        return true;
      }
    }

    currentNode = -1;
    step();
    return true;
  }

  void reset() {
    std::fill(visited.begin(), visited.end(), false);
    while (!q.empty())
      q.pop();

    for (Vertex &v : G->nodes) {
      v.color = BLACK;
    }
    currentNode = -1;
    neighborIndex = 0;
    visited[0] = true;
    G->nodes[0].color = GREEN;
    q.push(0);
  }
};

int main() {
  // initialize
  InitWindow(800, 600, "graph");
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
  G.nodes.push_back({350, 400, 20, BLACK, "D", {5}}); // D -> F
  // E = 4
  G.nodes.push_back({120, 500, 20, BLACK, "E", {5}}); // E -> F
  // F = 5
  G.nodes.push_back({650, 450, 20, BLACK, "F", {}}); // F has no outgoing edges

  Block B;
  BFSrunner bfs(&G);
  bool wasDown = false;
  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();
    ClearBackground(Hex_to_deci("ffffff"));
    B.draw();
    G.draw();
    Next.draw_tri();
    Reset.draw_rec();
    bool nowDown =
        Next.isPressed_tri(); // When release left mouse, nowDown = false;
                              // It will skip the following if statement and
                              // finally wasDown = false
    if (nowDown && !wasDown) {
      bfs.step();
    }
    wasDown = nowDown;
    if (Reset.isPressed_rec())
      bfs.reset();
    // end drawing
    EndDrawing();
  }

  // close window
  CloseWindow();
}
