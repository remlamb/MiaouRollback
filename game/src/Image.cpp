#include "Image.h"

using namespace raylib;
void ImageCustom::Setup(const char* path, float scale, Pivot mode) {
  sprite = LoadTexture(path);
  originalScale = scale;
  pivot = mode;
}

void ImageCustom::SetupIcon(const char* path, Pivot mode) {
  icon = LoadImage(path);
  pivot = mode;
}

void ImageCustom::TearDown() { UnloadTexture(sprite); }

void ImageCustom::Draw(Vector2 position) const {
  if (pivot == Pivot::Center) {
    position.x -= sprite.width * originalScale * 0.5f;
    position.y -= sprite.height * originalScale * 0.5f;
  }
  DrawTextureEx(sprite, position, 0.f, originalScale, WHITE);
}

void ImageCustom::Draw(Vector2 position, float scale) {
  if (pivot == Pivot::Center) {
    position.x -= sprite.width * originalScale * scale * 0.5f;
    position.y -= sprite.height * originalScale * scale * 0.5f;
  }
  DrawTextureEx(sprite, position, 0.f, originalScale * scale, WHITE);
}