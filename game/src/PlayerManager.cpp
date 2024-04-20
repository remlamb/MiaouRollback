#include "PlayerManager.h"
#include <iostream>

PlayerManager::PlayerManager(Physics::World* world_) : world_(world_) {}

void PlayerManager::SetUp() {
  world_->contactListener = this;
  players[0].position = Math::Vec2F(50, 650);
  players[1].position = Math::Vec2F(550, 650);
  trigger_nbrs_.fill(0);
  int it = 0;
  for (auto& player : players) {
    Physics::BodyRef bodyRef = world_->CreateBody();
    auto& newBody = world_->GetBody(bodyRef);
    newBody.SetMass(1);
    newBody.SetPosition(player.position);
    newBody.SetVelocity(Math::Vec2F(0, 0));
    newBody.type = Physics::BodyType::DYNAMIC;

    players_BodyRefs_[it] = bodyRef;

    Physics::ColliderRef colliderRef = world_->CreateCollider(bodyRef);
    auto& newCollider = world_->GetCollider(colliderRef);
    newCollider._shape = Math::ShapeType::Circle;
    newCollider.isTrigger = false;
    newCollider.restitution = 0.0f;
    newCollider.circleShape.SetRadius(collider_radius_);
    newCollider.ID = collider_id_++;

    players_CollidersRefs_[it] = colliderRef;

    Physics::BodyRef groundedBodyRef = world_->CreateBody();
    auto& groundedBody = world_->GetBody(groundedBodyRef);
    groundedBody.SetMass(1);
    groundedBody.SetPosition(newBody.Position() -
                             Math::Vec2F(grounded_collider_dimension_.X * 0.5f,
                                         grounded_collider_pos_y_));
    groundedBody.SetVelocity(Math::Vec2F(0, 0));
    groundedBody.type = Physics::BodyType::STATIC;

    players_grounded_BodyRefs_[it] = groundedBodyRef;

    Physics::ColliderRef colliderGroundedRef =
        world_->CreateCollider(players_grounded_BodyRefs_[it]);
    auto& groundedCollider = world_->GetCollider(colliderGroundedRef);
    groundedCollider._shape = Math::ShapeType::Rectangle;
    groundedCollider.isTrigger = true;
    groundedCollider.ID = collider_id_++;
    groundedCollider.restitution = 0.0f;
    groundedCollider.rectangleShape.SetMinBound({0.0f, 0.0f});
    groundedCollider.rectangleShape.SetMaxBound(grounded_collider_dimension_);
    players_grounded_CollidersRefs_[it] = colliderGroundedRef;
    it++;
  }
}

void PlayerManager::Update() {
  int it = 0;
  for (auto& player : players) {
    auto& collider = world_->GetCollider(players_CollidersRefs_[it]);
    const auto position = world_->GetBody(players_BodyRefs_[it]).Position();
    switch (collider._shape) {
      case Math::ShapeType::Rectangle:
        collider.rectangleShape =
            Math::RectangleF(position + collider.rectangleShape.MinBound(),
                             position + collider.rectangleShape.MaxBound() -
                                 collider.rectangleShape.MinBound());
        break;
      case Math::ShapeType::Circle:
        collider.circleShape =
            Math::CircleF(position, collider.circleShape.Radius());
        break;
      default:
        break;
    }

    auto& body = world_->GetBody(players_BodyRefs_[it]);
    players[it].position = body.Position();
    // groundedColliderObj.UpdateCollider();
    auto& groundedBody = world_->GetBody(players_grounded_BodyRefs_[it]);
    groundedBody.SetPosition(player.position -
                             Math::Vec2F(grounded_collider_dimension_.X * 0.5f,
                                         grounded_collider_pos_y_));

    // groundedColliderObj.UpdateCollider();
    auto& groundedCollider =
        world_->GetCollider(players_grounded_CollidersRefs_[it]);
    const auto& groundedPosition =
        world_->GetBody(players_grounded_BodyRefs_[it]).Position();
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

    player.is_grounded = trigger_nbrs_[it] > 1;
    if (!player.is_grounded) {
      body.AddForce({0, gravity_});
    } else {
      // for the rope
      body.AddForce({0, rope_gravity_});
    }

    it++;
  }
}

void PlayerManager::Jump(int playerIdx) {
  auto& body = world_->GetBody(players_BodyRefs_[playerIdx]);
  if (players[playerIdx].is_grounded) {
    body.SetVelocity({body.Velocity().X, jump_velocity_});
  }
}
void PlayerManager::Move(bool rightDirection, int playerIdx) {
  auto& body = world_->GetBody(players_BodyRefs_[playerIdx]);
  float targetVelocityX = rightDirection ? move_velocity_ : -move_velocity_;
  body.SetVelocity(Math::Vec2F::Lerp(body.Velocity(),
                                     {targetVelocityX, body.Velocity().Y},
                                     acceleration_time_));
}

void PlayerManager::Decelerate(int playerIdx) {
  auto& body = world_->GetBody(players_BodyRefs_[playerIdx]);
  const Math::Vec2F zeroVelocity = {0.0f, body.Velocity().Y};
  body.SetVelocity(
      Math::Vec2F::Lerp(body.Velocity(), zeroVelocity, deceleration_time_));
}

void PlayerManager::DrawDebug() {
  int it = 0;
  for (auto player : players) {
    auto& curent_collider = world_->GetCollider(players_CollidersRefs_[it]);
    auto& body = world_->GetBody(players_BodyRefs_[it]);
    switch (curent_collider._shape) {
      case Math::ShapeType::Rectangle:
        raylib::DrawRectangleLines(
            body.Position().X + curent_collider.rectangleShape.MinBound().X,
            body.Position().Y + curent_collider.rectangleShape.MinBound().Y,
            curent_collider.rectangleShape.MaxBound().X -
                curent_collider.rectangleShape.MinBound().X,
            curent_collider.rectangleShape.MaxBound().Y -
                curent_collider.rectangleShape.MinBound().Y,
            raylib::PURPLE);
        break;
      case Math::ShapeType::Circle:
        raylib::DrawCircleLines(body.Position().X, body.Position().Y,
                                curent_collider.circleShape.Radius(),
                                raylib::PURPLE);
        break;
      default:
        break;
    }

    auto& curent_collider2 =
        world_->GetCollider(players_grounded_CollidersRefs_[it]);
    auto& body2 = world_->GetBody(players_BodyRefs_[it]);
    raylib::Color color = raylib::PURPLE;
    if (curent_collider2.isTrigger) {
      color = raylib::BLUE;
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
  if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[0]) ||
      colliderB == world_->GetCollider(players_grounded_CollidersRefs_[0])) {
    trigger_nbrs_[0]++;
  }

  if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[1]) ||
      colliderB == world_->GetCollider(players_grounded_CollidersRefs_[1])) {
    trigger_nbrs_[1]++;
  }
}
void PlayerManager::OnTriggerExit(Physics::Collider colliderA,
                                  Physics::Collider colliderB) noexcept {
  if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[0]) ||
      colliderB == world_->GetCollider(players_grounded_CollidersRefs_[0])) {
    trigger_nbrs_[0]--;
  }

  if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[1]) ||
      colliderB == world_->GetCollider(players_grounded_CollidersRefs_[1])) {
    trigger_nbrs_[1]--;
  }
}
void PlayerManager::OnCollisionEnter(Physics::Collider colliderA,
                                     Physics::Collider colliderB) noexcept {}
void PlayerManager::OnCollisionExit(Physics::Collider colliderA,
                                    Physics::Collider colliderB) noexcept {}
