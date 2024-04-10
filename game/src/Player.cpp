#include "Player.h"

#include <iostream>


Player::Player(Engine::World& world_) : world(world_) {}

void Player::SetUp() {
  colliderObj.CreateCircleColliderObject(position, ColliderRadius, 1, false,
                                         Engine::BodyType::DYNAMIC);
  auto& collider = world.GetCollider(colliderObj.colliderRef);
  auto& body = world.GetBody(colliderObj.bodyRef);
  collider.restitution = 0.0f;

  groundedColliderObj.CreateRectangleColliderObject(
      Math::Vec2F(body.Position() -
                  Math::Vec2F(groundedColliderDimension.X * 0.5f,
                              groundedColliderPosY)),
      {0.0f, 0.0f}, groundedColliderDimension, 1, true,
      Engine::BodyType::DYNAMIC);
}

void Player::Update() {
  // player
  auto& body = world.GetBody(colliderObj.bodyRef);
  if (!isGrounded) {
    body.AddForce({0, gravity});
  } else {
    // for the rope
    body.AddForce({0, ropeGravity});
  }

  if (groundedColliderObj.CollisionNbr > 1) {
    isGrounded = true;
  } else {
    isGrounded = false;
  }

  auto& groundedCollider = world.GetBody(groundedColliderObj.bodyRef);
  groundedCollider.SetPosition(
      body.Position() -
      Math::Vec2F(groundedColliderDimension.X * 0.5f, groundedColliderPosY));

  position = body.Position();
}

void Player::Jump() {
  auto& body = world.GetBody(colliderObj.bodyRef);
  if (isGrounded) {
    body.SetVelocity({body.Velocity().X, jumpVelocity});
  }
}
void Player::Move(bool rightDirection) {
  auto& body = world.GetBody(colliderObj.bodyRef);
  float targetVelocityX = rightDirection ? moveVelocity : -moveVelocity;
  body.SetVelocity(Math::Vec2F::Lerp(
      body.Velocity(), {targetVelocityX, body.Velocity().Y}, accelerationTime));
}

void Player::Decelerate() {
  auto& body = world.GetBody(colliderObj.bodyRef);
  const Math::Vec2F zeroVelocity = {0.0f, body.Velocity().Y};
  body.SetVelocity(
      Math::Vec2F::Lerp(body.Velocity(), zeroVelocity, decelerationTime));
}
