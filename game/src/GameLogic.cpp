#include "GameLogic.h"

#include <iostream>

#include "Random.h"

void GameLogic::Init() noexcept {
  timer_.OnStart();
  world_.Init();
  world_.contactListener = this;

  // Border
  ColliderObject border{world_};
  border.CreateRectangleColliderObject({0, screenHeight - borderSize},
                                       {0.0, 0.0}, {screenWidth, borderSize}, 1,
                                       false, Engine::BodyType::STATIC);
  Colliders.emplace_back(border);

  ColliderObject border2{world_};
  ColliderObject border3{world_};
  ColliderObject border4{world_};
  border2.CreateRectangleColliderObject({screenWidth - borderSize, 0},
                                        {0.0, 0.0}, {borderSize, screenHeight},
                                        1, false, Engine::BodyType::STATIC);
  border3.CreateRectangleColliderObject({0, 0}, {0.0, 0.0},
                                        {screenWidth, borderSize}, 1, false,
                                        Engine::BodyType::STATIC);
  border4.CreateRectangleColliderObject({0, 0}, {0.0, 0.0},
                                        {borderSize, screenHeight}, 1, false,
                                        Engine::BodyType::STATIC);
  Colliders.emplace_back(border2);
  Colliders.emplace_back(border3);
  Colliders.emplace_back(border4);

  ColliderObject platform{world_};
  ColliderObject platform1{world_};
  ColliderObject platform2{world_};
  // Platform
  platform.CreateRectangleColliderObject(
      {250 - static_cast<float>(platformSize.X * 0.5), screenHeight - 150},
      {0.0, 0.0}, platformSize, 1, false, Engine::BodyType::STATIC);
  platform2.CreateRectangleColliderObject(
      {screenWidth - 250 - static_cast<float>(platformSize.X * 0.5),
       screenHeight - 150},
      {0.0, 0.0}, platformSize, 1, false, Engine::BodyType::STATIC);
  platform1.CreateRectangleColliderObject(
      {static_cast<float>(screenWidth * 0.5) -
           static_cast<float>(platformSize.X * 0.5),
       screenHeight - 300},
      {0.0, 0.0}, platformSize, 1, false, Engine::BodyType::STATIC);
  Colliders.emplace_back(platform);
  Colliders.emplace_back(platform1);
  Colliders.emplace_back(platform2);

  // Rope
  ColliderObject rope{world_};
  ColliderObject rope1{world_};
  rope.CreateRectangleColliderObject(
      {450 - static_cast<float>(20.0f * 0.5), screenHeight - 650}, {0.0, 0.0},
      {20.0f, 250.0f}, 1, true, Engine::BodyType::STATIC);

  rope1.CreateRectangleColliderObject(
      {screenWidth - 450 - static_cast<float>(20.0f * 0.5), screenHeight - 650},
      {0.0, 0.0}, {20.0f, 250.0f}, 1, true, Engine::BodyType::STATIC);
  Colliders.emplace_back(rope);
  Colliders.emplace_back(rope1);

  for (int i = 0; i < 3; i++) {
    ColliderObject newColliderObj{world_};
    newColliderObj.CreateCircleColliderObject(
        {250 + static_cast<float>(i * 2), 650 + static_cast<float>(i * 2.5)},
        20.0f, 100, false, Engine::BodyType::DYNAMIC);
    auto& ccollider = world_.GetCollider(newColliderObj.colliderRef);
    auto& cbody = world_.GetBody(newColliderObj.bodyRef);
    cbody.SetVelocity(Math::Vec2F(80.0f, 120.0f));
    ccollider.restitution = 0.0f;
    Colliders.emplace_back(newColliderObj);
  }

  player.SetUp();
  Colliders.emplace_back(player.colliderObj);
  Colliders.emplace_back(player.groundedColliderObj);
}

void GameLogic::Update() noexcept {
  // Player Input
  ManageInput();

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

  world_.Update(1/50.0f);
  player.Update();
}

void GameLogic::DeInit() noexcept {
  world_.Clear();

  world_.contactListener = nullptr;
  Colliders.empty();
}

void GameLogic::ManageInput() noexcept {
  inputs.SetPlayerInputs();
  if (inputs.playerInputs & static_cast<std::uint8_t>(Input::kJump)) {
    player.Jump();
  }
  if (inputs.playerInputs & static_cast<std::uint8_t>(Input::kLeft)) {
    player.Move(false);
  } else if (inputs.playerInputs & static_cast<std::uint8_t>(Input::kRight)) {
    player.Move(true);
  } else {
    player.Decelerate();
  }
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