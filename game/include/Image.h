#pragma once
#include <raylib.h>

enum class Pivot { Default, Center };

class ImageCustom {
 public:
  Texture2D sprite;
  float originalScale;
  Pivot pivot;

  void Setup(const char* path, float scale, Pivot mode);
  void TearDown();
  void Draw(Vector2 position) const;
  void Draw(Vector2 position, float scale);
};
