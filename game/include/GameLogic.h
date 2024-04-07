#pragma once
#include <raylib.h>

#include "Timer.h"
#include "World.h"

struct ColliderObject {
  Engine::BodyRef bodyRef;
  Engine::ColliderRef colliderRef;
  Color color;
  int CollisionNbr = 0;
};

struct Player {
  ColliderObject collider;
  Math::Vec2F position;
  bool isGrounded = false;
  ColliderObject groundedCollider;
  static constexpr Math::Vec2F groundedColliderDimension{10.0f, 10.0f};
};

class GameLogic : public Engine::ContactListener {
 public:
  Engine::World world_;
  Engine::Timer timer_;

  Player player;

  std::vector<Engine::BodyRef> _bodyRefs;
  std::vector<Engine::ColliderRef> _colRefs;

  int colliderCurrentID = 0;
  static constexpr std::size_t CirclesInTheWorld = 50;
  std::vector<ColliderObject> Colliders;

  static constexpr float VelocityMaxOnStart = 10.0f;
  static constexpr float BorderSizeForElements = 50.0f;
  void Init() noexcept;
  void Update() noexcept;
  void DeInit() noexcept;

  void Jump();

  ColliderObject CreateColliderObject(Math::Vec2F position, float mass,
                                      bool isTrigger, Math::ShapeType shape,
                                      Engine::BodyType type) noexcept;
  void CreateCircleColliderObject(Math::Vec2F position, float radius,
                                  float mass, bool isTrigger,
                                  Engine::BodyType type) noexcept;
  void CreateRectangleColliderObject(Math::Vec2F position,
                                     Math::Vec2F rectMinBound,
                                     Math::Vec2F rectMaxBound, float mass,
                                     bool isTrigger,
                                     Engine::BodyType type) noexcept;
  void RenderColliderObject() noexcept;

  void OnTriggerEnter(Engine::Collider colliderA,
                      Engine::Collider colliderB) noexcept override;
  void OnTriggerExit(Engine::Collider colliderA,
                     Engine::Collider colliderB) noexcept override;
  void OnCollisionEnter(Engine::Collider colliderA,
                        Engine::Collider colliderB) noexcept override;
  void OnCollisionExit(Engine::Collider colliderA,
                       Engine::Collider colliderB) noexcept override;
};
