#pragma once
#include "GameLogic.h"
#include "Image.h"

class Renderer {
 public:
  Renderer(GameLogic* gl) : game_logic(gl) {}
  void Init() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

 private:
  void DrawColliderShape() noexcept;
  void DrawPlatforms() noexcept;
  void DrawPlayer() noexcept;
  void DrawBackground() noexcept;
  void DrawLimit() noexcept;

  GameLogic* game_logic = nullptr;

  ImageCustom img;
  ImageCustom player;
  ImageCustom playerWeapon;
  ImageCustom background;
  ImageCustom platform;

  ImageCustom borderBottom;
  ImageCustom borderLeft;
  ImageCustom borderRight;
  ImageCustom borderTop;
  Image icon;
  float customScale = 0.0f;
  Vector2 center = {GameLogic::screenWidth * 0.5f,
                    GameLogic::screenHeight * 0.5f};
};