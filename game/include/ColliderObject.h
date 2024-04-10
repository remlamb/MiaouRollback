#pragma once
#include <raylib.h>

#include "Collider.h"
#include "World.h"

class ColliderObject {
 private:
  Engine::World& world;
  Color color = WHITE;

 public:
  ColliderObject(Engine::World& world_) : world(world_) {}

  void CreateColliderObject(Math::Vec2F position, float mass, bool isTrigger,
                            Math::ShapeType shape,
                            Engine::BodyType type) noexcept;
  void CreateCircleColliderObject(Math::Vec2F position, float radius,
                                  float mass, bool isTrigger,
                                  Engine::BodyType type) noexcept;
  void CreateRectangleColliderObject(Math::Vec2F position,
                                     Math::Vec2F rectMinBound,
                                     Math::Vec2F rectMaxBound, float mass,
                                     bool isTrigger,
                                     Engine::BodyType type) noexcept;
  void RenderColliderObject() const noexcept;

  Engine::BodyRef bodyRef;
  Engine::ColliderRef colliderRef;
  int CollisionNbr = 0;
};
