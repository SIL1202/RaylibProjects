#include "../include/raylib.h"
#include "../include/raymath.h"
#include "Global.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

void DrawArrow(Vector2 start, Vector2 end, float arrowSize, Color color);
const float MIN_DISTANCE = 80.0f; // 節點之間的最小距離（含半徑）

struct Vertex {
  int centerX, centerY;
  float r;
  Color color;
  std::string text;
  std::vector<int> neighbors;

  void draw() const {
    DrawCircle(centerX, centerY, r, color);
    int textWidth = MeasureText(text.c_str(), 20);
    DrawText(text.c_str(), (centerX - (float)textWidth / 2),
             (float)centerY - 10, 20, Hex_to_deci("212121"));
  }
};

struct Graph {
  std::vector<Vertex> nodes;
  void generateRandomNodes(int n, int screenWidth, int screenHeight) {
    nodes.clear();
    const float MIN_DISTANCE = 80.0f;
    const int MAX_ATTEMPTS = 100;

    // 計算有效區域時考慮節點半徑
    int minX = Margin.left + 50; // 左邊界 + 緩衝區
    int maxX = screenWidth - Margin.right - 50;
    int minY = Margin.top + 50;
    int maxY = screenHeight - Margin.bottom - 150;

    // 1. 生成節點位置
    for (int i = 0; i < n; i++) {
      int x, y;
      float r = GetRandomValue(20, 40);
      bool valid;
      int attempts = 0;

      do {
        valid = true;
        x = GetRandomValue(minX + r, maxX - r); // 確保節點不會超出邊界
        y = GetRandomValue(minY + r, maxY - r);

        // 檢查與現有節點的距離
        for (const Vertex &existing : nodes) {
          float dx = x - existing.centerX;
          float dy = y - existing.centerY;
          float minSeparation = r + existing.r + MIN_DISTANCE;
          if (dx * dx + dy * dy < minSeparation * minSeparation) {
            valid = false;
            break;
          }
        }

        if (++attempts >= MAX_ATTEMPTS) {
          std::cerr << "Warning: 無法為節點 " << i << " 找到合適位置\n";
          // 強制放置但標記為紅色

          nodes.push_back({x, y, r, RED, std::string(1, 'A' + i), {}});
          valid = true; // 跳出循環
        }
      } while (!valid);

      if (attempts < MAX_ATTEMPTS) {
        nodes.push_back(
            {x, y, r, COLOR_UNVISITED, std::string(1, 'A' + i), {}});
      }
    }

    // 2. 生成連接（優先連接近的節點）
    for (int i = 0; i < n; i++) {
      std::vector<std::pair<float, int>> candidates; // (distance, index)

      // 收集所有候選節點
      for (int j = 0; j < n; j++) {
        if (i == j)
          continue;

        float dx = nodes[i].centerX - nodes[j].centerX;
        float dy = nodes[i].centerY - nodes[j].centerY;
        candidates.emplace_back(dx * dx + dy * dy, j);
      }

      // 按距離排序
      std::sort(candidates.begin(), candidates.end());

      // 隨機決定連接數 (1~3)
      int maxEdges = GetRandomValue(1, std::min(3, (int)candidates.size()));

      // 選擇最近的 maxEdges 個節點
      for (int e = 0; e < maxEdges; e++) {
        nodes[i].neighbors.push_back(candidates[e].second);

        // 確保連接是雙向的 (可選)
        if (std::find(nodes[candidates[e].second].neighbors.begin(),
                      nodes[candidates[e].second].neighbors.end(),
                      i) == nodes[candidates[e].second].neighbors.end()) {
          nodes[candidates[e].second].neighbors.push_back(i);
        }
      }
    }
  }

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

        DrawArrow(from, to, 10.0f, Hex_to_deci("424242"));
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
  std::vector<const char *> blocks;

public:
  Block() { blocks.resize(6, nullptr); }

  void enqueue(const char *text) {
    for (int i = 0; i < blocks.size(); i++) {
      if (blocks[i] == nullptr) {
        blocks[i] = text;
        break;
      }
    }
  }

  void dequeue() {
    for (int i = 0; i < blocks.size(); i++) {
      if (blocks[i] != nullptr) {
        // remove first occupied
        for (int j = i; j < blocks.size() - 1; j++) {
          blocks[j] = blocks[j + 1];
        }
        blocks.back() = nullptr;
        break;
      }
    }
  }

  void draw() {
    for (int i = 0; i < blocks.size(); i++) {
      Rectangle rect = {(float)(50 + i * 40), (float)(GetScreenHeight() - 60),
                        40, 40};
      DrawRectangleRec(rect, Hex_to_deci("F5F5F5"));
      DrawRectangleLinesEx(rect, 2, Hex_to_deci("9E9E9E"));
      if (blocks[i]) {
        float textWidth = MeasureText(blocks[i], 20);
        DrawText(blocks[i], rect.x + rect.width / 2 - textWidth / 2,
                 rect.y + rect.height / 2 - 10, 20, Hex_to_deci("212121"));
      }
    }
  }

  void clear() { std::fill(blocks.begin(), blocks.end(), nullptr); }
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

  /* BFSrunner(Graph *g) : G(g), visited(g->nodes.size(), false) {
    reset(); // Initialize with reset
  } */
  BFSrunner(Graph *g) : G(g), visited(g->nodes.size(), false) {
    int randomStart = GetRandomValue(0, G->nodes.size() - 1); // 隨機起始點
    visited[randomStart] = true;
    G->nodes[randomStart].color = COLOR_VISITED;
    q.push(randomStart);
  }

  int currentNode = -1;
  size_t neighborIndex = 0;

  int step() {
    // If we're processing neighbors of a node
    if (currentNode != -1) {
      auto &neighbors = G->nodes[currentNode].neighbors;
      while (neighborIndex < neighbors.size()) {
        int neighbor = neighbors[neighborIndex++];
        if (!visited[neighbor]) {
          visited[neighbor] = true;
          G->nodes[neighbor].color = COLOR_VISITED;
          q.push(neighbor);
          std::cout << "push: " << G->nodes[neighbor].text << std::endl;
          return neighbor;
        }
      }

      // Done processing all neighbors
      std::cout << "pop: " << G->nodes[currentNode].text << std::endl;
      currentNode = -1;
      neighborIndex = 0;
    }

    // Get next node from queue
    if (!q.empty()) {
      currentNode = q.front();
      q.pop();
      std::cout << "pop: " << G->nodes[currentNode].text << std::endl;
      neighborIndex = 0;
      return -1; // Return -1 to indicate we're processing a new node
    }

    // Queue is empty, traversal complete
    return -2; // Indicate traversal is complete
  }

  void reset() {
    std::fill(visited.begin(), visited.end(), false);
    while (!q.empty())
      q.pop();

    for (Vertex &v : G->nodes) {
      v.color = COLOR_UNVISITED;
    }

    currentNode = -1;
    neighborIndex = 0;

    // Start with node 0
    visited[0] = true;
    G->nodes[0].color = COLOR_VISITED;
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
  /*  Graph G;
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
  */
  Graph G;
  G.generateRandomNodes(6, GetScreenWidth(), GetScreenHeight());

  Block B;
  B.enqueue(G.nodes[0].text.c_str());
  BFSrunner bfs(&G);
  bool wasDown = false;
  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();
    ClearBackground(Hex_to_deci("FAFAFA"));
    B.draw();
    G.draw();
    Next.draw_tri();
    Reset.draw_rec();
    bool nowDown =
        Next.isPressed_tri(); // When release left mouse, nowDown = false;
                              // It will skip the following if statement and
    if (nowDown && !wasDown) {
      int id = bfs.step();
      if (id == -2) {
        // Traversal complete
        std::cout << "BFS traversal complete!\n";
      } else if (id >= 0) {
        B.enqueue(G.nodes[id].text.c_str());
      } else if (id == -1) {
        B.dequeue();
      }
    }
    wasDown = nowDown;
    if (Reset.isPressed_rec()) {
      std::cout << "-------reset-------\n";
      bfs.reset();
      B.clear();
    }

    // end drawing
    EndDrawing();
  }

  // close window
  CloseWindow();
}
