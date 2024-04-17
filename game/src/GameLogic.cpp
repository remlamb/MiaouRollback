#include "GameLogic.h"

#include <iostream>

#include "Random.h"

void GameLogic::Init() noexcept {
  timer_.OnStart();
  world_.Init();
  player.SetUp();
  // Border
  game::Collider border{&world_};
  border.CreateRectangleColliderObject({0, screenHeight - borderSize},
                                       {0.0, 0.0}, {screenWidth, borderSize}, 1,
                                       false, Physics::BodyType::STATIC);
  Colliders.emplace_back(border);

  game::Collider border2{&world_};
  game::Collider border3{&world_};
  game::Collider border4{&world_};
  border2.CreateRectangleColliderObject({screenWidth - borderSize, 0},
                                        {0.0, 0.0}, {borderSize, screenHeight},
                                        1, false, Physics::BodyType::STATIC);
  border3.CreateRectangleColliderObject({0, 0}, {0.0, 0.0},
                                        {screenWidth, borderSize}, 1, false,
                                        Physics::BodyType::STATIC);
  border4.CreateRectangleColliderObject({0, 0}, {0.0, 0.0},
                                        {borderSize, screenHeight}, 1, false,
                                        Physics::BodyType::STATIC);
  Colliders.emplace_back(border2);
  Colliders.emplace_back(border3);
  Colliders.emplace_back(border4);

  game::Collider platform{&world_};
  game::Collider platform1{&world_};
  game::Collider platform2{&world_};
  // Platform
  platform.CreateRectangleColliderObject(
      {250 - static_cast<float>(platformSize.X * 0.5), screenHeight - 150},
      {0.0, 0.0}, platformSize, 1, false, Physics::BodyType::STATIC);
  platform2.CreateRectangleColliderObject(
      {screenWidth - 250 - static_cast<float>(platformSize.X * 0.5),
       screenHeight - 150},
      {0.0, 0.0}, platformSize, 1, false, Physics::BodyType::STATIC);
  platform1.CreateRectangleColliderObject(
      {static_cast<float>(screenWidth * 0.5) -
           static_cast<float>(platformSize.X * 0.5),
       screenHeight - 300},
      {0.0, 0.0}, platformSize, 1, false, Physics::BodyType::STATIC);
  Colliders.emplace_back(platform);
  Colliders.emplace_back(platform1);
  Colliders.emplace_back(platform2);

  // Rope
  game::Collider rope{&world_};
  game::Collider rope1{&world_};
  rope.CreateRectangleColliderObject(
      {450 - static_cast<float>(20.0f * 0.5), screenHeight - 650}, {0.0, 0.0},
      {20.0f, 250.0f}, 1, true, Physics::BodyType::STATIC);

  rope1.CreateRectangleColliderObject(
      {screenWidth - 450 - static_cast<float>(20.0f * 0.5), screenHeight - 650},
      {0.0, 0.0}, {20.0f, 250.0f}, 1, true, Physics::BodyType::STATIC);
  Colliders.emplace_back(rope);
  Colliders.emplace_back(rope1);

  for (int i = 0; i < 3; i++) {
    game::Collider newColliderObj{&world_};
    newColliderObj.CreateCircleColliderObject(
        {250 + static_cast<float>(i * 2), 650 + static_cast<float>(i * 2.5)},
        20.0f, 100, false, Physics::BodyType::DYNAMIC);
    auto& ccollider = world_.GetCollider(newColliderObj.colliderRef);
    auto& cbody = world_.GetBody(newColliderObj.bodyRef);
    cbody.SetVelocity(Math::Vec2F(80.0f, 120.0f));
    ccollider.restitution = 0.0f;
    Colliders.emplace_back(newColliderObj);
  }


}

void GameLogic::Update() noexcept {
  // PlayerManager Input
  ManageInput();

  for (auto& colliderObj : Colliders) {
    colliderObj.UpdateCollider();
  }
  player.Update();
  world_.Update(1 / 50.0f);
}

void GameLogic::DeInit() noexcept {
  world_.Clear();

  world_.contactListener = nullptr;
  Colliders.empty();
}

void GameLogic::ManageInput() noexcept {
  inputs.SetPlayerInputs();
  if (inputs.playerInput & static_cast<std::uint8_t>(Input::kJump)) {
    player.Jump(0);
  }
  if (inputs.playerInput & static_cast<std::uint8_t>(Input::kLeft)) {
    player.Move(false, 0);
  } else if (inputs.playerInput & static_cast<std::uint8_t>(Input::kRight)) {
    player.Move(true, 0);
  } else {
    player.Decelerate(0);
  }

  //Todo Remove Temporary for local
  if (IsKeyDown(KEY_UP)) {
    player.Jump(1);
  }
  if (IsKeyDown(KEY_LEFT)) {
    player.Move(false, 1);
  } else if (IsKeyDown(KEY_RIGHT)) {
    player.Move(true, 1);
  } else {
    player.Decelerate(1);
  }

}

