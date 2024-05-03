#pragma once
#include "Constants.h"
#include "FrameInput.h"
#include "World.h"
#include "raylib_wrapper.h"
using namespace raylib;

/**
 * @brief Represents the Player position, and event that must be shared with
 * other client. The collider struct consists of three members:
 * - `position`:  Reference the position of the player at the current frame.
 * - `is_grounded`: Reference if the player is or isn't touching the ground at
 * the current frame.
 * - `velocity`: Reference to the player movement speed in the physical world.
 */

struct Player {
  bool is_grounded = false;
  bool is_projectile_ready = true;
  int trigger_nbr = 0;
  int life_point = 3;
  float attack_timer = 0.0f;
  std::uint8_t input;
};

struct Projectile {
  Physics::BodyRef projectile_body;
  Physics::ColliderRef projectile_collider;
  int current_collider_nbr = 0;
  int nbr_launching_player = 0;
  // is a way to split projectile not currently in the game and the real one,
  // should used physical engine system but i have a bug
  bool isActive = false;
};

/**
 * @brief Manages player entities in the game, including physics interactions,
 * movement, and input handling.
 *
 * The PlayerManager class handles the creation and update of player entities
 * within the game world. It manages player physics bodies, colliders, and
 * interactions with the game environment. This class also implements collision
 * and trigger event handling for player entities.
 */
class PlayerManager : public Physics::ContactListener {
 private:
  Physics::World* world_;
  static constexpr float jump_velocity_ = -580.0f;
  static constexpr float move_velocity_ = 500.0f;
  static constexpr float collider_radius_ = 28.0f;
  static constexpr float grounded_collider_pos_y_ = -20.0f;
  static constexpr Math::Vec2F grounded_collider_dimension_{10.0f, 10.0f};

  static constexpr float acceleration_time_ = 0.04f;
  static constexpr float deceleration_time_ = 0.1f;

  static constexpr int nbr_player_ = game::max_player;
  static constexpr int player1_collider_id_ = 1;
  static constexpr int player2_collider_id_ = 2;

  static constexpr int player1_groundedcollider_id_ = 3;
  static constexpr int player2_groundedcollider_id_ = 4;

  static constexpr Math::Vec2F player1_spawn_pos_ = {250, 550};
  static constexpr Math::Vec2F player2_spawn_pos_ = {game::screen_width - 250,
                                                     550};

  static constexpr float projectile_radius_ = 24.0f;
  static constexpr int projectile_id_ = 20;
  static constexpr int neutral_projectile_id_ = 18;
  static constexpr Math::Vec2F projectile_speed_ = Math::Vec2F(0, 480);
  static constexpr float time_between_attack = 1.0f;

  int current_projectile_collider_id_ = projectile_id_;
  // std::array<int, nbr_player_> trigger_nbrs_;
  // int collider_id_ = 0;




 public:
  static constexpr std::int16_t max_projectile_ = 80;
  std::array<Projectile, max_projectile_> projectiles_{};
  static constexpr float gravity_ = 1000;
  static constexpr float rope_gravity_ = 10;
  PlayerManager(Physics::World* world_);

  /**
   * @brief Create player entities within the game world.
   */
  void SetUp();
  /**
   * @brief Updates player entities and their interactions.
   */
  void Update();

  /**
   * @brief Initiates a jump action for a specified player.
   * @param playerIdx Index of the player.
   */
  void Jump(int playerIdx);
  /**
   * @brief Initiates a movement action for a specified player.
   * @param rightDirection Flag indicating the movement direction (true for
   * right, false for left).
   * @param playerIdx Index of the player.
   */
  void Move(bool rightDirection, int playerIdx);
  /**
   * @brief Initiates a deceleration action for a specified player.
   * @param playerIdx Index of the player.
   */
  void Decelerate(int playerIdx);

  void Attack(int player_idx);

  Math::Vec2F GetPlayerPosition(int idx) const noexcept;

  void OnTriggerEnter(Physics::Collider colliderA,
                      Physics::Collider colliderB) noexcept override;
  void OnTriggerExit(Physics::Collider colliderA,
                     Physics::Collider colliderB) noexcept override;
  void OnCollisionEnter(Physics::Collider colliderA,
                        Physics::Collider colliderB) noexcept override;
  void OnCollisionExit(Physics::Collider colliderA,
                       Physics::Collider colliderB) noexcept override;

  std::array<Player, nbr_player_> players;
  // projectile confirmé
  std::vector<Projectile> old_projectiles_;
  // add vector projectile detruit/noncomfirmé

  std::array<Physics::BodyRef, nbr_player_> players_BodyRefs_;
  std::array<Physics::ColliderRef, nbr_player_> players_CollidersRefs_;

  std::array<Physics::BodyRef, nbr_player_> players_grounded_BodyRefs_;
  std::array<Physics::ColliderRef, nbr_player_> players_grounded_CollidersRefs_;

  void InitProjectiles();
  void GetNewProjectile(int player_idx);
  Math::Vec2F GetProjectilePosition(int idx) const noexcept;
  void ResetProjectiles();
};
