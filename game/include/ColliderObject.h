#pragma once
#include <raylib.h>
#include "Collider.h"
#include "World.h"

class ColliderObject {
 public:
  ColliderObject() = default;
  Engine::BodyRef bodyRef;
  Engine::ColliderRef colliderRef;
  Color color = WHITE;
  int CollisionNbr = 0;

  void CreateColliderObject(Math::Vec2F position, float mass, bool isTrigger,
                            Math::ShapeType shape, Engine::BodyType type,
                            Engine::World& world_) noexcept;
  void CreateCircleColliderObject(Math::Vec2F position, float radius,
                                  float mass, bool isTrigger,
                                  Engine::BodyType type,
                                  Engine::World& world_) noexcept;
  void CreateRectangleColliderObject(Math::Vec2F position,
                                     Math::Vec2F rectMinBound,
                                     Math::Vec2F rectMaxBound, float mass,
                                     bool isTrigger, Engine::BodyType type,
                                     Engine::World& world_) noexcept;
  void RenderColliderObject(Engine::World& world_) noexcept;
};
