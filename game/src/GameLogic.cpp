#include "GameLogic.h"

#include <iostream>

#include "Random.h"

namespace game {
void GameLogic::Init() noexcept {
  timer_.OnStart();
  world_.Init();
  player_manager.SetUp();

  //// Border
  CreatePlatform({0, 0}, {0.0, 0.0}, {screenWidth, border_size_});
  CreatePlatform({0, 0}, {0.0, 0.0}, {border_size_, screenHeight});
  CreatePlatform({0, screenHeight - border_size_}, {0, 0},
                 {screenWidth, border_size_});
  CreatePlatform({screenWidth - border_size_, 0}, {0, 0},
                 {border_size_, screenHeight});

  ////Platform
  CreatePlatform(
      {250 - static_cast<float>(platform_size_.X * 0.5), screenHeight - 150},
      {0.0, 0.0}, {platform_size_});
  CreatePlatform(
      {screenWidth - 250 - static_cast<float>(platform_size_.X * 0.5),
       screenHeight - 150},
      {0.0, 0.0}, {platform_size_});
  CreatePlatform(
      {static_cast<float>(screenWidth * 0.5) -
           static_cast<float>(platform_size_.X * 0.5),
       static_cast<float>(platform_size_.X * 0.5) + screenHeight * 0.5f},
      {0, 0}, {platform_size_});

  //// Rope
  CreateRope({450 - static_cast<float>(20.0f * 0.5), screenHeight - 650},
             {0.0, 0.0}, {20.0f, 250.0f});
  CreateRope(
      {screenWidth - 450 - static_cast<float>(20.0f * 0.5), screenHeight - 650},
      {0.0, 0.0}, {20.0f, 250.0f});
}

void GameLogic::Update() noexcept {
  if (current_game_state != GameState::GameLaunch) {
    return;
  }

  world_.Update(fixedUpdateFrenquency);
  // PlayerManager Input
  ManageInputAndUpdateGameplay();
  UpdateCollider();
  player_manager.Update();
}

void GameLogic::DeInit() noexcept {
  world_.Clear();

  world_.contactListener = nullptr;
  colliders_.empty();
}

void GameLogic::ManageInputAndUpdateGameplay() noexcept {
  inputs.UpdatePlayerInputs();
  if (client_player_nbr == invalid_client_player_nbr) {
    return;
  }

  if (inputs.playerInput & static_cast<std::uint8_t>(Input::kJump)) {
    player_manager.Jump(client_player_nbr);
  }
  if (inputs.playerInput & static_cast<std::uint8_t>(Input::kLeft)) {
    player_manager.Move(false, client_player_nbr);
  } else if (inputs.playerInput & static_cast<std::uint8_t>(Input::kRight)) {
    player_manager.Move(true, client_player_nbr);
  } else {
    player_manager.Decelerate(client_player_nbr);
  }
}

void GameLogic::CreatePlatform(Math::Vec2F position, Math::Vec2F rectMinBound,
                               Math::Vec2F rectMaxBound) noexcept {
  Physics::BodyRef bodyRef = world_.CreateBody();
  auto& newBody = world_.GetBody(bodyRef);
  newBody.SetMass(10);
  newBody.SetPosition(position);
  newBody.SetVelocity(Math::Vec2F(0, 0));
  newBody.type = Physics::BodyType::STATIC;

  Physics::ColliderRef colliderRef = world_.CreateCollider(bodyRef);
  auto& newCollider = world_.GetCollider(colliderRef);
  newCollider._shape = Math::ShapeType::Rectangle;
  newCollider.isTrigger = false;
  newCollider.restitution = 0.0f;
  newCollider.rectangleShape.SetMinBound(rectMinBound);
  newCollider.rectangleShape.SetMaxBound(rectMaxBound);
  newCollider.ID = platform_collider_id_;
  colliders_.emplace_back(collider{bodyRef, colliderRef});
}

void GameLogic::CreateRope(Math::Vec2F position, Math::Vec2F rectMinBound,
                           Math::Vec2F rectMaxBound) noexcept {
  Physics::BodyRef bodyRef = world_.CreateBody();
  auto& newBody = world_.GetBody(bodyRef);
  newBody.SetMass(1);
  newBody.SetPosition(position);
  newBody.SetVelocity(Math::Vec2F(0, 0));
  newBody.type = Physics::BodyType::STATIC;

  Physics::ColliderRef colliderRef = world_.CreateCollider(bodyRef);
  auto& newCollider = world_.GetCollider(colliderRef);
  newCollider._shape = Math::ShapeType::Rectangle;
  newCollider.isTrigger = true;
  newCollider.restitution = 0.0f;
  newCollider.rectangleShape.SetMinBound(rectMinBound);
  newCollider.rectangleShape.SetMaxBound(rectMaxBound);
  newCollider.ID = rope_collider_id_;
  colliders_.emplace_back(collider{bodyRef, colliderRef});
}

void GameLogic::UpdateCollider() noexcept {
  for (auto& collider : colliders_) {
    auto& physicsCollider = world_.GetCollider(collider.colliderRef);
    auto& physicsBody = world_.GetBody(collider.bodyRef);
    switch (physicsCollider._shape) {
      case Math::ShapeType::Rectangle:
        physicsCollider.rectangleShape = Math::RectangleF(
            physicsBody.Position(),
            physicsBody.Position() + physicsCollider.rectangleShape.MaxBound() -
                physicsCollider.rectangleShape.MinBound());
        break;
      case Math::ShapeType::Circle:
        physicsCollider.circleShape = Math::CircleF(
            physicsBody.Position(), physicsCollider.circleShape.Radius());
        break;
      default:
        break;
    }
  }
}
}  // namespace game
