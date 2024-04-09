#include "Player.h"

void Player::SetUp(Engine::World& world) {
  // player

  // CreateRectangleColliderObject({ 50 , 650 }, { 0.0, 0.0 }, { 20.0, 20.0 },
  // 100, false, Engine::BodyType::DYNAMIC);
  colliderObj.CreateCircleColliderObject({50, 650}, 28.0, 100, false,Engine::BodyType::DYNAMIC, world);


  // colliderObj = Colliders[colliderCurrentID - 1];
  auto& collider = world.GetCollider(colliderObj.colliderRef);
  auto& body = world.GetBody(colliderObj.bodyRef);
  collider.restitution = 0.0f;

  groundedColliderObj.CreateRectangleColliderObject(
      Math::Vec2F(body.Position() -
                  Math::Vec2F(groundedColliderDimension.X * 0.5f, -20.0f)),
      {0.0f, 0.0f}, groundedColliderDimension, 1, true,
      Engine::BodyType::DYNAMIC, world);
}

void Player::Update(Engine::World& world) {
  // player
  auto& body = world.GetBody(colliderObj.bodyRef);
  if (!isGrounded) {
    body.AddForce({0, 100000});
  } else {
    // for the rope
    body.AddForce({0, 1000});
  }

  if (groundedColliderObj.CollisionNbr > 1) {
    isGrounded = true;
  } else {
    isGrounded = false;
  }

  auto& groundedCollider = world.GetBody(groundedColliderObj.bodyRef);
  groundedCollider.SetPosition(
      body.Position() - Math::Vec2F(groundedColliderDimension.X * 0.5f, -20.0f));
}

void Player::Jump(Engine::World& world) {
  auto& body = world.GetBody(colliderObj.bodyRef);
  if (isGrounded) {
    body.SetVelocity({body.Velocity().X, -580.0f});
  }
}
void Player::Move(bool rightDirection, Engine::World& world) {
  auto& body = world.GetBody(colliderObj.bodyRef);
  float targetVelocityX = rightDirection ? 500.0f : -500.0f;

  float acceleration = 0.04f;
  body.SetVelocity(Math::Vec2F::Lerp(
      body.Velocity(), {targetVelocityX, body.Velocity().Y}, acceleration));
}

void Player::Decelerate(Engine::World& world) {
  auto& body = world.GetBody(colliderObj.bodyRef);
  Math::Vec2F zeroVelocity = {0.0f, body.Velocity().Y};
  float deceleration = 0.04f;
  body.SetVelocity(
      Math::Vec2F::Lerp(body.Velocity(), zeroVelocity, deceleration));
}
