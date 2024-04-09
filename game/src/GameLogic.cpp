#include "GameLogic.h"

#include <iostream>

#include "Random.h"

void GameLogic::Init() noexcept {
  timer_.OnStart();
  world_.Init();
  world_.contactListener = this;

  // Border
  ColliderObject border;
  border.CreateRectangleColliderObject({0, screenHeight - borderSize},
                                       {0.0, 0.0}, {screenWidth, borderSize}, 1,
                                       false, Engine::BodyType::STATIC, world_);
  Colliders.emplace_back(border);

  ColliderObject border2;
  ColliderObject border3;
  ColliderObject border4;
  border2.CreateRectangleColliderObject(
      {screenWidth - borderSize, 0}, {0.0, 0.0}, {borderSize, screenHeight}, 1,
      false, Engine::BodyType::STATIC, world_);
  border3.CreateRectangleColliderObject({0, 0}, {0.0, 0.0},
                                        {screenWidth, borderSize}, 1, false,
                                        Engine::BodyType::STATIC, world_);
  border4.CreateRectangleColliderObject({0, 0}, {0.0, 0.0},
                                        {borderSize, screenHeight}, 1, false,
                                        Engine::BodyType::STATIC, world_);
  Colliders.emplace_back(border2);
  Colliders.emplace_back(border3);
  Colliders.emplace_back(border4);

  ColliderObject platform;
  ColliderObject platform1;
  ColliderObject platform2;
  // Platform
  platform.CreateRectangleColliderObject(
      {250 - static_cast<float>(platformSize.X * 0.5), screenHeight - 150},
      {0.0, 0.0}, platformSize, 1, false, Engine::BodyType::STATIC, world_);
  platform2.CreateRectangleColliderObject(
      {screenWidth - 250 - static_cast<float>(platformSize.X * 0.5),
       screenHeight - 150},
      {0.0, 0.0}, platformSize, 1, false, Engine::BodyType::STATIC, world_);
  platform1.CreateRectangleColliderObject(
      {static_cast<float>(screenWidth * 0.5) -
           static_cast<float>(platformSize.X * 0.5),
       screenHeight - 300},
      {0.0, 0.0}, platformSize, 1, false, Engine::BodyType::STATIC, world_);
  Colliders.emplace_back(platform);
  Colliders.emplace_back(platform1);
  Colliders.emplace_back(platform2);

  // Rope
  ColliderObject rope;
  ColliderObject rope1;
  rope.CreateRectangleColliderObject(
      {450 - static_cast<float>(20.0f * 0.5), screenHeight - 650}, {0.0, 0.0},
      {20.0f, 250.0f}, 1, true, Engine::BodyType::STATIC, world_);

  rope1.CreateRectangleColliderObject(
      {screenWidth - 450 - static_cast<float>(20.0f * 0.5), screenHeight - 650},
      {0.0, 0.0}, {20.0f, 250.0f}, 1, true, Engine::BodyType::STATIC, world_);
  Colliders.emplace_back(rope);
  Colliders.emplace_back(rope1);

  // for (int i = 0; i < 50; i++) {
  //   CreateCircleColliderObject(
  //       {250 + static_cast<float>(i * 2), 650 + static_cast<float>(i * 2.5)},
  //       20.0f, 100, false, Engine::BodyType::DYNAMIC);
  //   auto& ccollider =
  //       world.GetCollider(Colliders[colliderCurrentID - 1].colliderRef);
  //   auto& cbody = world.GetBody(Colliders[colliderCurrentID - 1].bodyRef);
  //   cbody.SetVelocity(Math::Vec2F(80.0f, 120.0f));
  //   ccollider.restitution = 1.0f;
  // }

  player.SetUp(world_);
  Colliders.emplace_back(player.colliderObj);
  Colliders.emplace_back(player.groundedColliderObj);
}

void GameLogic::Update() noexcept {
  for (auto& colliderObj : Colliders) {
    auto& collider = world_.GetCollider(colliderObj.colliderRef);
    const auto position = world_.GetBody(colliderObj.bodyRef).Position();
    switch (collider._shape) {
      case Math::ShapeType::Rectangle:
        collider.rectangleShape = Math::RectangleF(
            position, position + collider.rectangleShape.MaxBound() -
                          collider.rectangleShape.MinBound());
        break;
      case Math::ShapeType::Circle:
        collider.circleShape =
            Math::CircleF(position, collider.circleShape.Radius());
        break;
      default:
        break;
    }
  }
  world_.Update(timer_.DeltaTime());
  player.Update(world_);
}

void GameLogic::DeInit() noexcept {
  _bodyRefs.clear();
  _colRefs.clear();
  world_.Clear();

  world_.contactListener = nullptr;
  Colliders.empty();
}

void GameLogic::OnTriggerEnter(Engine::Collider colliderA,
                               Engine::Collider colliderB) noexcept {
  auto groundedCollider =
      world_.GetCollider(player.groundedColliderObj.colliderRef);
  if (groundedCollider == colliderA || groundedCollider == colliderB) {
    player.groundedColliderObj.CollisionNbr++;
  }
}
void GameLogic::OnTriggerExit(Engine::Collider colliderA,
                              Engine::Collider colliderB) noexcept {
  auto groundedCollider =
      world_.GetCollider(player.groundedColliderObj.colliderRef);
  if (groundedCollider == colliderA || groundedCollider == colliderB) {
    player.groundedColliderObj.CollisionNbr--;
  }
}
void GameLogic::OnCollisionEnter(Engine::Collider colliderA,
                                 Engine::Collider colliderB) noexcept {}
void GameLogic::OnCollisionExit(Engine::Collider colliderA,
                                Engine::Collider colliderB) noexcept {}