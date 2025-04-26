#include "Botton.hpp"
#include "Global.hpp"
#include "TextInput.hpp"

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

// one thing in to-do-list
struct thing {
  int width, height;
  int x, y;
  std::string text;
  void draw() const {
    DrawRectangle(x, y, width, height, HexToColor("FFC3CC"));
    int textWidth = MeasureText(text.c_str(), 30);
    float textX = x + (float)width / 2 - (float)textWidth / 2;
    float textY = y + (float)height / 4;
    Vector2 pos{textX, textY};
    DrawTextEx(myfont, text.c_str(), pos, 30, 2, HexToColor("28301C"));
  }
};

// independently adding to-do-list's ractangle
void AddRectangle(std::vector<thing> &ToDo, std::string text) {
  if (!ToDo.empty()) {
    if (ToDo[ToDo.size() - 1].y + 50 >= GetScreenHeight() - Margin.bottom) {
      return;
    }
    thing todo{.width = 400,
               .height = 40,
               .x = ToDo[ToDo.size() - 1].x,
               .y = ToDo[ToDo.size() - 1].y + 50,
               .text = text};
    ToDo.emplace_back(todo);
    return;
  }
  ToDo.emplace_back(thing{400, 40, GetScreenWidth() / 2 - 400 / 2, 30, text});
}

int main() {
  // initial
  InitWindow(800, 600, "To-Do-List");
  InitFont();
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  std::vector<thing> toDoList;
  TextInput inputBox;
  ExpandBotton addbtn(
      {static_cast<float>(GetScreenWidth() - Margin.right - 25),
       static_cast<float>(GetScreenHeight() - Margin.bottom - 25)},
      25, HexToColor("28301C"), "../assets/PlusIcon.png",
      GetScreenWidth() - Margin.right - Margin.left);

  while (!WindowShouldClose()) {
    // update
    /* --------------handling adding system -----------------*/
    // show botton animation
    if (addbtn.isHovered()) {
      addbtn.color = HexToColor("BBBEB5"); // 變淺
    } else {                               // reset botton position
      addbtn.color = HexToColor("28301C"); // 回復
    }

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

    // drawing
    BeginDrawing();
    ClearBackground(HexToColor("28301C"));
    // AddBotton.draw();
    for (const auto &list : toDoList)
      list.draw();
    addbtn.draw();
    inputBox.draw();

    // end of drawing
    EndDrawing();
  }

  // close
  CloseWindow();
  return 0;
}
