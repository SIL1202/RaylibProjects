#include "Botton.hpp"
#include "Global.hpp"

CircleBotton::CircleBotton(Vector2 p, float r, Color c, const char *iconPath)
    : pos(p), r(r), color(c) {
  icon = LoadTexture(iconPath);
  icon.width = icon.width * 0.0625;
  icon.height = icon.height * 0.0625;
}

bool CircleBotton::isHovered() const {
  return CheckCollisionPointCircle(GetMousePosition(), this->pos, this->r);
}
bool CircleBotton::isClicked() const {
  return CheckCollisionPointCircle(GetMousePosition(), this->pos, this->r) &&
             IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
         IsKeyPressed(KEY_TAB);
}

// drawing original circle botton and texture icon
void CircleBotton::draw() const {
  DrawCircleV(pos, r, color);
  if (icon.id != 0)
    DrawTexture(icon, pos.x - (float)icon.width / 2,
                pos.y - (float)icon.height / 2, WHITE);
}

CircleBotton::~CircleBotton() { UnloadTexture(icon); }

ExpandBotton::ExpandBotton(Vector2 p, float r, Color c, const char *iconPath,
                           float expandWidth)
    : CircleBotton(p, r, c, iconPath), isExpanding(false), isExpanded(false),
      isClosing(false), currentWidth(r * 2), targetWidth(expandWidth) {}

// update expanding animation
void ExpandBotton::update() {
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
void ExpandBotton::draw() const {
  if (isExpanded || isExpanding || isClosing) {
    float rightX = pos.x - r;
    float x = rightX - currentWidth + r * 2;
    Rectangle rec{.x = x,
                  .y = pos.y - this->r,
                  .width = this->currentWidth,
                  .height = r * 2};
    DrawRectangleRounded(rec, 1.0f, 32, HexToColor("FFC3CC"));
  } else
    CircleBotton::draw();
}
