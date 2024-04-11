#include "PlayerManager.h"

#include <iostream>

PlayerManager::PlayerManager(Physics::World* world_) : world(world_) {}

void PlayerManager::SetUp() {
  world->contactListener = this;
  for (auto player :players_)
  {
    colliderObj.CreateCircleColliderObject(player.position, ColliderRadius, 1, false,
                                           Physics::BodyType::DYNAMIC);
    auto& collider = world->GetCollider(colliderObj.colliderRef);
    auto& body = world->GetBody(colliderObj.bodyRef);
    collider.restitution = 0.0f;

    groundedColliderObj.CreateRectangleColliderObject(
        Math::Vec2F(body.Position() -
                    Math::Vec2F(groundedColliderDimension.X * 0.5f,
                                groundedColliderPosY)),
        {0.0f, 0.0f}, groundedColliderDimension, 1, true,
        Physics::BodyType::DYNAMIC);
  }
}

void PlayerManager::Update() {
  // TODO index du colliderObj au lieu du colliderObj
  colliderObj.UpdateCollider();
  groundedColliderObj.UpdateCollider();
  auto& body = world->GetBody(colliderObj.bodyRef);
  if (!isGrounded) {
    body.AddForce({0, gravity});
  } else {
    // for the rope
    body.AddForce({0, ropeGravity});
  }
  isGrounded = groundedColliderObj.TriggerNbr > 1;

  auto& groundedCollider = world->GetBody(groundedColliderObj.bodyRef);
  groundedCollider.SetPosition(
      body.Position() -
      Math::Vec2F(groundedColliderDimension.X * 0.5f, groundedColliderPosY));

  position = body.Position();
}

void PlayerManager::Jump() {
  auto& body = world->GetBody(colliderObj.bodyRef);
  if (isGrounded) {
    body.SetVelocity({body.Velocity().X, jumpVelocity});
  }
}
void PlayerManager::Move(bool rightDirection) {
  auto& body = world->GetBody(colliderObj.bodyRef);
  float targetVelocityX = rightDirection ? moveVelocity : -moveVelocity;
  body.SetVelocity(Math::Vec2F::Lerp(
      body.Velocity(), {targetVelocityX, body.Velocity().Y}, accelerationTime));
}

void PlayerManager::Decelerate() {
  auto& body = world->GetBody(colliderObj.bodyRef);
  const Math::Vec2F zeroVelocity = {0.0f, body.Velocity().Y};
  body.SetVelocity(
      Math::Vec2F::Lerp(body.Velocity(), zeroVelocity, decelerationTime));
}

void PlayerManager::DrawDebug() {
  groundedColliderObj.RenderColliderObject();
  colliderObj.RenderColliderObject();
}

void PlayerManager::OnTriggerEnter(Physics::Collider colliderA,
                                   Physics::Collider colliderB) noexcept {
  auto groundedCollider = world->GetCollider(groundedColliderObj.colliderRef);
  if (groundedCollider == colliderA || groundedCollider == colliderB) {
    groundedColliderObj.TriggerNbr++;
  }
}
void PlayerManager::OnTriggerExit(Physics::Collider colliderA,
                                  Physics::Collider colliderB) noexcept {
  auto groundedCollider = world->GetCollider(groundedColliderObj.colliderRef);
  if (groundedCollider == colliderA || groundedCollider == colliderB) {
    groundedColliderObj.TriggerNbr--;
  }
}
void PlayerManager::OnCollisionEnter(Physics::Collider colliderA,
                                     Physics::Collider colliderB) noexcept {}
void PlayerManager::OnCollisionExit(Physics::Collider colliderA,
                                    Physics::Collider colliderB) noexcept {}
