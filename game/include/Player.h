#pragma once
#include "ColliderObject.h"

class Player {

 public:
  Player() = default;
  //Engine::World world;
  ColliderObject colliderObj;
  Math::Vec2F position = {0.0f, 0.0f};
  bool isGrounded = false;
  ColliderObject groundedColliderObj;
  static constexpr Math::Vec2F groundedColliderDimension{10.0f, 10.0f};

  void SetUp(Engine::World& world);
  void Update(Engine::World& world);

  void Jump(Engine::World& world);
  void Move(bool rightDirection, Engine::World& world);
  void Decelerate(Engine::World& world);
};
