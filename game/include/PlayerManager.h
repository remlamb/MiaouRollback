#pragma once
#include "Collider.h"

// TODO pointeur au lieu de ref
// Pas copier les ptr pour le rollback
// split player manger et player struct

// player manager -> on trigger ext

struct Player {
  Math::Vec2F position = {50, 650};  // in pixels
  bool isGrounded = false;
  Math::Vec2F velocity = {0, 0};
};

class PlayerManager : public Physics::ContactListener {
 private:
  Physics::World* world;

  static constexpr float gravity = 1000;
  static constexpr float ropeGravity = 10;
  static constexpr float jumpVelocity = -580.0f;
  static constexpr float moveVelocity = 500.0f;
  static constexpr float ColliderRadius = 28.0f;
  static constexpr float groundedColliderPosY = -20.0f;
  static constexpr Math::Vec2F groundedColliderDimension{10.0f, 10.0f};

  static constexpr float accelerationTime = 0.04f;
  static constexpr float decelerationTime = 0.1f;

  static constexpr int nbrPlayer = 2;

  std::array<Physics::BodyRef, nbrPlayer> playersBodyRefs;
  std::array<Physics::ColliderRef, nbrPlayer> playersCollidersRefs;

  std::array<Physics::BodyRef, nbrPlayer> playersGroundedBodyRefs;
  std::array<Physics::ColliderRef, nbrPlayer> playersGroundedCollidersRefs;
  std::array<int, nbrPlayer> TriggerNbrs;

 public:
  PlayerManager(Physics::World* world_);

  void SetUp();
  void Update();

  void Jump(int playerIdx);
  void Move(bool rightDirection, int playerIdx);
  void Decelerate(int playerIdx);

  void DrawDebug();

  void OnTriggerEnter(Physics::Collider colliderA,
                      Physics::Collider colliderB) noexcept override;
  void OnTriggerExit(Physics::Collider colliderA,
                     Physics::Collider colliderB) noexcept override;
  void OnCollisionEnter(Physics::Collider colliderA,
                        Physics::Collider colliderB) noexcept override;
  void OnCollisionExit(Physics::Collider colliderA,
                       Physics::Collider colliderB) noexcept override;

  // Private
  // Collider Mananger et -> en debug pour dessiner seulement

  std::array<Player, nbrPlayer> players_;
};
