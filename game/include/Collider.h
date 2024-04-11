#pragma once
#include <raylib.h>

#include "Collider.h"
#include "World.h"

// TODO changer reference en pointeur;
// peut aussi faire un colliderManager -> une seul ref au world
// collider Manger puet evoir les trigger enter exit etc...
namespace game {
class Collider {
 private:
  Physics::World* world;
  Color color = WHITE;

 public:
  Collider(Physics::World* world_) : world(world_) {}

  void CreateColliderObject(Math::Vec2F position, float mass, bool isTrigger,
                            Math::ShapeType shape,
                            Physics::BodyType type) noexcept;
  void CreateCircleColliderObject(Math::Vec2F position, float radius,
                                  float mass, bool isTrigger,
                                  Physics::BodyType type) noexcept;
  void CreateRectangleColliderObject(Math::Vec2F position,
                                     Math::Vec2F rectMinBound,
                                     Math::Vec2F rectMaxBound, float mass,
                                     bool isTrigger,
                                     Physics::BodyType type) noexcept;

  void UpdateCollider() const;
  void RenderColliderObject() const noexcept;

  Physics::BodyRef bodyRef;
  Physics::ColliderRef colliderRef;
  int CollisionNbr = 0;
  int TriggerNbr = 0;
  int index = 0;
};
}  // namespace game
