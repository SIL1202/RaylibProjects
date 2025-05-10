#include "Botton.hpp"
#include "Global.hpp"
#include "TextInput.hpp"
#include <string>
#include <utility>

// extern 在這裡真正定義，才會在記憶體上開空間
Font myfont;
UILayout Margin;

Color HexToColor(const std::string &hex) {
  if (hex.size() != 6 && hex.size() != 8) {
    std::cerr << "❌ Hex color format error: " << hex << "\n";
    return WHITE;
  }

  unsigned int r = std::stoi(hex.substr(0, 2), nullptr, 16);
  unsigned int g = std::stoi(hex.substr(2, 2), nullptr, 16);
  unsigned int b = std::stoi(hex.substr(4, 2), nullptr, 16);
  unsigned int a =
      (hex.size() == 8) ? std::stoi(hex.substr(6, 2), nullptr, 16) : 255;

  return Color{(unsigned char)r, (unsigned char)g, (unsigned char)b,
               (unsigned char)a};
}

void InitFont() {
  myfont = LoadFontEx("../assets/JetBrainsMonoNerdFont-Bold.ttf", 32, 0, 250);
  if (myfont.texture.id == 0) {
    std::cerr << "❌ Failed to load font!" << std::endl;
    exit(1);
  }
}

// independently adding to-do-list's ractangle
void AddRectangle(
    std::vector<std::pair<thing, std::unique_ptr<CircleBotton>>> &Object,
    const std::string &text) {
  float recX = static_cast<float>(Margin.left) * 2;
  float height = 40.f;
  float bottonRadius = 25.f;
  float bottonCenterX = GetScreenWidth() - Margin.right - bottonRadius;

  if (!Object.empty()) {
    float lowestY = Object.back().first.y + 50.f;
    if (lowestY >= GetScreenHeight() - Margin.bottom)
      return;
  }

  float startY = 40.f;
  float gap = 50.f;

  thing newThing{height, recX, 0.f, text};
  auto newBotton = std::make_unique<CircleBotton>(
      Vector2{bottonCenterX, 0.f}, bottonRadius,
      Fade(HexToColor("28301C"), 0.0f), "../assets/PlusIcon.png", 45.0f);
  Object.insert(Object.begin(), {newThing, std::move(newBotton)});

  for (size_t i = 0; i < Object.size(); ++i) {
    Object[i].first.y = startY + gap * i;
    Object[i].second->pos.y = startY + bottonRadius + gap * i;
    Object[i].second->rotation = 45.0f;
  }
}

int main() {
  // initial
  InitWindow(800, 600, "To-Do-List");
  InitFont();
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  std::vector<std::pair<thing, std::unique_ptr<CircleBotton>>> toDoList;

  TextInput inputBox;
  ExpandBotton addbtn(
      {static_cast<float>(GetScreenWidth() - Margin.right - 25),
       static_cast<float>(GetScreenHeight() - Margin.bottom - 25)},
      25, Fade(HexToColor("28301C"), 0.0f), "../assets/PlusIcon.png",
      GetScreenWidth() - Margin.right - Margin.left);

  while (!WindowShouldClose()) {
    // update
    /* --------------handling adding system -----------------*/
    // show botton animation

    if (addbtn.isClicked()) {
      addbtn.triggerExpand();
    }
    addbtn.update();

    // handling inputBox
    if (addbtn.isExpanded)
      inputBox.isActive = true;
    inputBox.update();
    if (inputBox.submit()) {
      addbtn.triggerClose();
      AddRectangle(toDoList, inputBox.value.c_str()); // adding list in here
      inputBox.value.clear();
    }
    /* --------------handling adding system -----------------*/

    /* --------------handling deleting system -----------------*/
    for (auto &botton : toDoList) {
      botton.second->update();
    }

    /* --------------handling deleting system -----------------*/

    // drawing
    BeginDrawing();
    ClearBackground(HexToColor("28301C"));
    // AddBotton.draw();
    for (auto &list : toDoList) {
      list.first.draw();   // draw list's rectangles
      list.second->draw(); // draw list's rectangles
    }
    addbtn.draw();
    inputBox.draw();

    // end of drawing
    EndDrawing();
  }

  // close
  CloseWindow();
  return 0;
}
