#pragma once
#include "GameLogic.h"
#include "Image.h"

class Renderer {
 public:
  Renderer(game::GameLogic* gl, NetworkLogic* network_logic)
      : game_logic_(gl), network_logic_(network_logic) {}
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

  game::GameLogic* game_logic_ = nullptr;
  NetworkLogic* network_logic_ = nullptr;

  ImageCustom player_;
  ImageCustom player2_;
  ImageCustom player_weapon_;
  ImageCustom background_;
  ImageCustom platform_;
  ImageCustom rope_;
  ImageCustom main_menu_bg_;

  ImageCustom border_bottom_;
  ImageCustom border_left_;
  ImageCustom border_right_;
  ImageCustom border_top_;
  ImageCustom icon_;
  raylib::Vector2 center_pos_ = {game::screen_width * 0.5f,
                            game::screen_height * 0.5f};
};
