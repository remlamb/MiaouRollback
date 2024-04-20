#pragma once
//#include "raylib.h"
#include "raylib_wrapper.h"

enum class Pivot { Default, Center };

class ImageCustom {
 public:
  raylib::Texture2D sprite;
  float originalScale;
  Pivot pivot;

  void Setup(const char* path, float scale, Pivot mode);
  void TearDown();
  void Draw(raylib::Vector2 position) const;
  void Draw(raylib::Vector2 position, float scale);
};
