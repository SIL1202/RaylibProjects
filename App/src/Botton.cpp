#include "Botton.hpp"
#include "Global.hpp"
#include <memory>

// TextureRAII 實現
TextureRAII::TextureRAII(const char *iconPath)
    : texture(LoadTexture(iconPath)) {
  if (texture.id == 0) {
    std::cerr << "❌ Failed to load icon: " << iconPath << std::endl;
  }
}

TextureRAII::~TextureRAII() {
  if (texture.id != 0) {
    UnloadTexture(texture);
  }
}

Texture2D TextureRAII::get() const { return texture; }

// CircleBotton 實現
CircleBotton::CircleBotton(Vector2 p, float r, Color c, const char *iconPath,
                           float initRotation)
    : pos(p), r(r), color(c), rotation(initRotation),
      icon(std::make_unique<TextureRAII>(iconPath)) {}

CircleBotton::CircleBotton(Vector2 p, float r, Color c)
    : pos(p), r(r), color(c), rotation(0.0f), icon(nullptr) {}

bool CircleBotton::isHovered() const {
  return CheckCollisionPointCircle(GetMousePosition(), pos, r);
}

bool CircleBotton::isClicked() const {
  return (CheckCollisionPointCircle(GetMousePosition(), pos, r) &&
          IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ||
         IsKeyPressed(KEY_TAB);
}

void CircleBotton::update() {
  // 旋轉控制：僅在懸停時旋轉
  if (isHovered()) {
    rotation += 0.1f; // 每幀旋轉 1 度
    if (rotation >= 360.f)
      rotation = 0.0f;
  } else {
    rotation -= 0.1f; // 退出懸停時重置
    if (rotation <= 0.0)
      rotation = 0.0f;
  }

  // 顏色控制
  color = isHovered() ? Fade(HexToColor("BBBEB5"), 1.0f)
                      : Fade(HexToColor("28301C"), 0.0f);
}

void CircleBotton::draw() {
  if (color.a > 0) {
    DrawCircleV(pos, r, color);
  }
  if (icon && icon->get().id != 0) {
    float scale = 0.0625f;
    Texture2D texture = icon->get();
    Rectangle source = {0, 0, (float)texture.width, (float)texture.height};
    Rectangle dest = {pos.x, pos.y, (float)texture.width * scale,
                      (float)texture.height * scale};
    Vector2 origin = {dest.width / 2, dest.height / 2};
    DrawTexturePro(texture, source, dest, origin, rotation,
                   HexToColor("FFC3CC"));
  }
}

CircleBotton::~CircleBotton() {
  // 不需要手動 UnloadTexture，TextureRAII 會自動處理
}

ExpandBotton::ExpandBotton(Vector2 p, float r, Color c, const char *iconPath,
                           float expandWidth)
    : CircleBotton(p, r, c, iconPath), isExpanding(false), isExpanded(false),
      isClosing(false), currentWidth(r * 2), targetWidth(expandWidth) {}

// update expanding animation
void ExpandBotton::update() {
  CircleBotton::update();
  if (isExpanding && currentWidth < targetWidth) {
    currentWidth += 1.5;
    if (currentWidth >= targetWidth) {
      currentWidth = targetWidth;
      isExpanding = false;
      isExpanded = true;
    }
  }
  if (isClosing && !isExpanded) {
    currentWidth -= 1.5;
    if (currentWidth <= r * 2) {
      currentWidth = r * 2;
      isClosing = false;
    }
  }
}

void ExpandBotton::triggerExpand() {
  if (!isExpanded) {
    isExpanding = true;
  }
}

void ExpandBotton::triggerClose() {
  if (isExpanded) {
    isClosing = true;
    isExpanded = false;
  }
}

// drawing rounded ractangle
void ExpandBotton::draw() {
  if (isExpanded || isExpanding || isClosing) {
    float rightX = pos.x - r;
    float x = rightX - currentWidth + r * 2;
    Rectangle rec{.x = x,
                  .y = pos.y - this->r,
                  .width = this->currentWidth,
                  .height = r * 2};
    DrawRectangleRounded(rec, 1.0f, 32, HexToColor("FFC3CC"));
  } else {
    CircleBotton::draw();
  }
}
