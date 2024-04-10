#pragma once
#include "ColliderObject.h"

class Player {
   private:
    Engine::World& world;

   static constexpr float gravity = 1000;
   static constexpr float ropeGravity = 10;
   static constexpr float jumpVelocity = -580.0f;
   static constexpr float moveVelocity = 500.0f;
   static constexpr float ColliderRadius = 28.0f;
   static constexpr float groundedColliderPosY = -20.0f;
   static constexpr Math::Vec2F groundedColliderDimension{10.0f, 10.0f};

   static constexpr float accelerationTime = 0.04f;
   static constexpr float decelerationTime = 0.1f;

 public:
  Player(Engine::World& world_);

  void SetUp();
  void Update();

  void Jump();
  void Move(bool rightDirection);
  void Decelerate();

  bool isGrounded = false;
  ColliderObject colliderObj{world};
  ColliderObject groundedColliderObj{world};
  Math::Vec2F position = {50, 650};
};
