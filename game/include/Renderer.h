#pragma once
#include "GameLogic.h"
#include "Image.h"

class Renderer {
 public:
  Renderer(game::GameLogic* gl) : game_logic(gl) {}
  void Init() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

 private:
  void DrawColliderShape() noexcept;
  void DrawPlatforms() noexcept;
  void DrawRopes() noexcept;
  void DrawPlayer() noexcept;
  void DrawBackground() noexcept;
  void DrawLimit() noexcept;

  game::GameLogic* game_logic = nullptr;

  ImageCustom player;
  ImageCustom player2;
  ImageCustom playerWeapon;
  ImageCustom background;
  ImageCustom platform;
  ImageCustom rope;

  ImageCustom borderBottom;
  ImageCustom borderLeft;
  ImageCustom borderRight;
  ImageCustom borderTop;
  raylib::Image icon;
  float customScale = 0.0f;
  raylib::Vector2 center = {
	  game::GameLogic::screenWidth * 0.5f,
	  game::GameLogic::screenHeight * 0.5f};
};
