#pragma once
#include "GameLogic.h"
#include "Image.h"

class Renderer {
 public:
  Renderer(game::GameLogic* gl) : game_logic(gl) {}
  void Init() noexcept;
  void Draw(bool isColliderVisible) noexcept;
  void Deinit() noexcept;

 private:
  /**
   * @brief Renders the colliders form in the game window, use for debugging.
   */
  void DrawColliderShape() noexcept;
  /**
   * @brief Draws collider form for debug information for player entities.
   */
  void DrawPlayerColliderShape() noexcept;
  void DrawPlatforms() noexcept;
  void DrawRopes() noexcept;
  void DrawPlayer() noexcept;
  void DrawProjectiles() noexcept;
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
  ImageCustom icon;
  raylib::Vector2 center = {
	  game::GameLogic::screenWidth * 0.5f,
	  game::GameLogic::screenHeight * 0.5f};
};
