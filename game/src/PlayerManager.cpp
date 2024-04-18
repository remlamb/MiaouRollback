#include "PlayerManager.h"

#include <iostream>

PlayerManager::PlayerManager(Physics::World* world_) : world(world_) {}

void PlayerManager::SetUp() {
  world->contactListener = this;
  players_[0].position = Math::Vec2F(50, 650);
  players_[1].position = Math::Vec2F(550, 650);
  TriggerNbrs.fill(0);
  int it = 0;
  for (auto& player : players_) {
    Physics::BodyRef bodyRef = world->CreateBody();
    auto& newBody = world->GetBody(bodyRef);
    newBody.SetMass(1);
    newBody.SetPosition(player.position);
    newBody.SetVelocity(Math::Vec2F(0, 0));
    newBody.type = Physics::BodyType::DYNAMIC;

    playersBodyRefs[it] = bodyRef;

    // Todo Must be in gameLogic
    Physics::ColliderRef colliderRef = world->CreateCollider(bodyRef);
    auto& newCollider = world->GetCollider(colliderRef);
    newCollider._shape = Math::ShapeType::Circle;
    newCollider.isTrigger = false;
    newCollider.restitution = 0.0f;
    newCollider.circleShape.SetRadius(ColliderRadius);
    newCollider.ID = ColliderID++;

    playersCollidersRefs[it] = colliderRef;

    // groundedColliderObj.CreateRectangleColliderObject(
    //     Math::Vec2F(newBody.Position() -
    //                 Math::Vec2F(groundedColliderDimension.X * 0.5f,
    //                             groundedColliderPosY)),
    //     {0.0f, 0.0f}, groundedColliderDimension, 1, true,
    //     Physics::BodyType::DYNAMIC);

    Physics::BodyRef groundedBodyRef = world->CreateBody();
    auto& groundedBody = world->GetBody(groundedBodyRef);
    groundedBody.SetMass(1);
    groundedBody.SetPosition(
        newBody.Position() -
        Math::Vec2F(groundedColliderDimension.X * 0.5f, groundedColliderPosY));
    groundedBody.SetVelocity(Math::Vec2F(0, 0));
    groundedBody.type = Physics::BodyType::STATIC;

    playersGroundedBodyRefs[it] = groundedBodyRef;

    // Todo Must be in gameLogic
    Physics::ColliderRef colliderGroundedRef =
        world->CreateCollider(playersGroundedBodyRefs[it]);
    auto& groundedCollider = world->GetCollider(colliderGroundedRef);
    groundedCollider._shape = Math::ShapeType::Rectangle;
    // groundedCollider._shape = Math::ShapeType::Circle;
    groundedCollider.isTrigger = true;
    groundedCollider.ID = ColliderID++;
    groundedCollider.restitution = 0.0f;
    groundedCollider.rectangleShape.SetMinBound({0.0f, 0.0f});
    groundedCollider.rectangleShape.SetMaxBound(groundedColliderDimension);
    // groundedCollider.circleShape.SetRadius(1.0f);
    playersGroundedCollidersRefs[it] = colliderGroundedRef;
    it++;
  }
}

void PlayerManager::Update() {
  int it = 0;
  for (auto& player : players_) {
    auto& collider = world->GetCollider(playersCollidersRefs[it]);
    const auto position = world->GetBody(playersBodyRefs[it]).Position();
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

    auto& body = world->GetBody(playersBodyRefs[it]);
    players_[it].position = body.Position();
    // groundedColliderObj.UpdateCollider();
    auto& groundedBody = world->GetBody(playersGroundedBodyRefs[it]);
    groundedBody.SetPosition(
        player.position -
        Math::Vec2F(groundedColliderDimension.X * 0.5f, groundedColliderPosY));

    // groundedColliderObj.UpdateCollider();
    auto& groundedCollider =
        world->GetCollider(playersGroundedCollidersRefs[it]);
    const auto& groundedPosition =
        world->GetBody(playersGroundedBodyRefs[it]).Position();
    switch (groundedCollider._shape) {
      case Math::ShapeType::Rectangle:
        groundedCollider.rectangleShape = Math::RectangleF(
            groundedPosition, groundedPosition +
                                  groundedCollider.rectangleShape.MaxBound() -
                                  groundedCollider.rectangleShape.MinBound());
        break;
      case Math::ShapeType::Circle:
        groundedCollider.circleShape =
            Math::CircleF(position, groundedCollider.circleShape.Radius());
        break;
      default:
        break;
    }

    player.isGrounded = TriggerNbrs[it] > 1;
    if (!player.isGrounded) {
      body.AddForce({0, gravity});
    } else {
      // for the rope
      body.AddForce({0, ropeGravity});
    }

    // isGrounded = groundedColliderObj.TriggerNbr > 1;

    it++;
  }
}

void PlayerManager::Jump(int playerIdx) {
  auto& body = world->GetBody(playersBodyRefs[playerIdx]);
  // std::cout << players_[playerIdx].isGrounded << std::endl;
  std::cout << TriggerNbrs[playerIdx] << std::endl;
  if (players_[playerIdx].isGrounded) {
    body.SetVelocity({body.Velocity().X, jumpVelocity});
  }
}
void PlayerManager::Move(bool rightDirection, int playerIdx) {
  auto& body = world->GetBody(playersBodyRefs[playerIdx]);
  float targetVelocityX = rightDirection ? moveVelocity : -moveVelocity;
  body.SetVelocity(Math::Vec2F::Lerp(
      body.Velocity(), {targetVelocityX, body.Velocity().Y}, accelerationTime));
}

void PlayerManager::Decelerate(int playerIdx) {
  auto& body = world->GetBody(playersBodyRefs[playerIdx]);
  const Math::Vec2F zeroVelocity = {0.0f, body.Velocity().Y};
  body.SetVelocity(
      Math::Vec2F::Lerp(body.Velocity(), zeroVelocity, decelerationTime));
}

void PlayerManager::DrawDebug() {
  // groundedColliderObj.RenderColliderObject();
  //  colliderObj.RenderColliderObject();
  int it = 0;
  for (auto player : players_) {
    auto& curent_collider = world->GetCollider(playersCollidersRefs[it]);
    auto& body = world->GetBody(playersBodyRefs[it]);
    switch (curent_collider._shape) {
      case Math::ShapeType::Rectangle:
        DrawRectangleLines(
            body.Position().X + curent_collider.rectangleShape.MinBound().X,
            body.Position().Y + curent_collider.rectangleShape.MinBound().Y,
            curent_collider.rectangleShape.MaxBound().X -
                curent_collider.rectangleShape.MinBound().X,
            curent_collider.rectangleShape.MaxBound().Y -
                curent_collider.rectangleShape.MinBound().Y,
            PURPLE);
        break;
      case Math::ShapeType::Circle:
        DrawCircleLines(body.Position().X, body.Position().Y,
                        curent_collider.circleShape.Radius(), PURPLE);
        break;
      default:
        break;
    }

    auto& curent_collider2 =
        world->GetCollider(playersGroundedCollidersRefs[it]);
    auto& body2 = world->GetBody(playersBodyRefs[it]);
    Color color = PURPLE;
    if (curent_collider2.isTrigger) {
      color = BLUE;
    }
    switch (curent_collider2._shape) {
      case Math::ShapeType::Rectangle:
        DrawRectangleLines(curent_collider2.rectangleShape.MinBound().X,
                           curent_collider2.rectangleShape.MinBound().Y,
                           curent_collider2.rectangleShape.MaxBound().X -
                               curent_collider2.rectangleShape.MinBound().X,
                           curent_collider2.rectangleShape.MaxBound().Y -
                               curent_collider2.rectangleShape.MinBound().Y,
                           color);
        break;
      case Math::ShapeType::Circle:
        DrawCircleLines(body2.Position().X, body2.Position().Y,
                        curent_collider2.circleShape.Radius(), color);
        break;
      default:
        break;
    }
    it++;
  }
}

void PlayerManager::OnTriggerEnter(Physics::Collider colliderA,
                                   Physics::Collider colliderB) noexcept {
  if (colliderA == world->GetCollider(playersGroundedCollidersRefs[0]) ||
      colliderB == world->GetCollider(playersGroundedCollidersRefs[0])) {
      TriggerNbrs[0]++;
  }

  if (colliderA == world->GetCollider(playersGroundedCollidersRefs[1]) ||
      colliderB == world->GetCollider(playersGroundedCollidersRefs[1])) {
      TriggerNbrs[1]++;
  }
}
void PlayerManager::OnTriggerExit(Physics::Collider colliderA,
                                  Physics::Collider colliderB) noexcept {
  if (colliderA == world->GetCollider(playersGroundedCollidersRefs[0]) ||
      colliderB == world->GetCollider(playersGroundedCollidersRefs[0])) {
      TriggerNbrs[0]--;
  }

  if (colliderA == world->GetCollider(playersGroundedCollidersRefs[1]) ||
      colliderB == world->GetCollider(playersGroundedCollidersRefs[1])) {
      TriggerNbrs[1]--;
  }
}
void PlayerManager::OnCollisionEnter(Physics::Collider colliderA,
                                     Physics::Collider colliderB) noexcept {}
void PlayerManager::OnCollisionExit(Physics::Collider colliderA,
                                    Physics::Collider colliderB) noexcept {}
