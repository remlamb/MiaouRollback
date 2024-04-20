#pragma once
#include "InputsManager.h"
#include "PlayerManager.h"
#include "Timer.h"
#include "World.h"
#include "raylib_wrapper.h"
using namespace raylib;

namespace game {
/**
 * @brief Represents a link between a physics object and its collider.
 * The collider struct consists of two members:
 * - `bodyRef`:  Reference to the physics body.
 * - `colliderRef`: Reference to the physics collider.
 *
 */
struct collider {
  Physics::BodyRef bodyRef;
  Physics::ColliderRef colliderRef;
};

/**
 * @brief This class manages the game logic, including physics simulation,
 * player management, and input handling. The `GameLogic` class initializes and
 * updates the game world, manages player input, and handles collision detection
 * and response. It creates platforms and ropes within the game world and
 * updates their associated colliders accordingly.
 *
 * The class contains a physics world, player manager, and inputs manager as
 * member variables. It also maintains constants for screen width and height, as
 * well as parameters for platform and rope creation.
 */
class GameLogic {
 public:
  Physics::World world_;
  PlayerManager player{&world_};
  InputsManager inputs;
  int currentClientPlayer = -1;

  static constexpr int screenWidth = 1480;
  static constexpr int screenHeight = 720;

  /**
   * @brief Init the game environment.
   */
  void Init() noexcept;
  /**
   * @brief Updates the game logic, colliders, inputs and apply inputs for
   * players.
   */
  void Update() noexcept;
  /**
   * @brief Deinit the game environment.
   */
  void DeInit() noexcept;

  /**
   * @brief Manages player inputs, link between the player and game inputs.
   */
  void ManageInput() noexcept;

  /**
   * @brief Creates a platform in the game world.
   * @param position The position of the platform.
   * @param rectMinBound The minimum bounds of the platform's rectangle.
   * @param rectMaxBound The maximum bounds of the platform's rectangle.
   */
  void CreatePlatform(Math::Vec2F position, Math::Vec2F rectMinBound,
                      Math::Vec2F rectMaxBound) noexcept;
  /**
   * @brief Creates a rope in the game world.
   * @param position The position of the rope.
   * @param rectMinBound The minimum bounds of the rope's rectangle.
   * @param rectMaxBound The maximum bounds of the rope's rectangle.
   */
  void GameLogic::CreateRope(Math::Vec2F position, Math::Vec2F rectMinBound,
                             Math::Vec2F rectMaxBound) noexcept;

  /**
   * @brief Updates the colliders in the game world.
   */
  void UpdateCollider() noexcept;
  /**
   * @brief Renders the colliders form in the game window, use for debugging.
   */
  void RenderColliderObject() noexcept;

 private:
  static constexpr int platform_collider_id_ = 10;
  static constexpr int rope_collider_id_ = 11;

  const float border_size_ = 20.0f;
  const Math::Vec2F platform_size_{200.0f, 40.0f};
  Physics::Timer timer_;
  std::vector<game::collider> colliders_;
};
}  // namespace game
