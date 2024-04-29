#pragma once
#include "raylib_wrapper.h"

enum class Pivot { Default, Center };

class ImageCustom {
 public:
  raylib::Texture2D sprite;
  float originalScale;
  Pivot pivot;

  raylib::Image icon;

  void Setup(const char* path, float scale, Pivot mode);
  void SetupIcon(const char* path, Pivot mode);
  void TearDown();
  void Draw(raylib::Vector2 position) const;
  void Draw(raylib::Vector2 position, float scale);
};
