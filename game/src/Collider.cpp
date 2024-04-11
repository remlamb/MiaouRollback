#include "Collider.h"

void game::Collider::CreateColliderObject(Math::Vec2F position, float mass,
                                          bool isTrigger, Math::ShapeType shape,
                                          Physics::BodyType type) noexcept {
  bodyRef = world->CreateBody();
  auto& newBody = world->GetBody(bodyRef);
  newBody.SetMass(mass);
  newBody.SetPosition(position);
  newBody.SetVelocity(Math::Vec2F(0, 0));
  newBody.type = type;
  colliderRef = world->CreateCollider(bodyRef);
  auto& newCollider = world->GetCollider(colliderRef);
  newCollider._shape = shape;
  newCollider.isTrigger = isTrigger;
  newCollider.restitution = 0.0f;
  switch (newBody.type) {
    case Physics::BodyType::DYNAMIC:
      if (newCollider.isTrigger) {
        color = BLUE;
      } else {
        color = RED;
      }
      break;
    case Physics::BodyType::STATIC:
      if (newCollider.isTrigger) {
        color = ORANGE;
      } else {
        color = YELLOW;
      }
      break;
    default:
      break;
  }
}

void game::Collider::CreateCircleColliderObject(
    Math::Vec2F position, float radius, float mass, bool isTrigger,
    Physics::BodyType type) noexcept {
  CreateColliderObject(position, mass, isTrigger, Math::ShapeType::Circle,
                       type);
  auto& collider = world->GetCollider(colliderRef);
  auto& body = world->GetBody(bodyRef);
  collider.circleShape.SetRadius(radius);
  index++;
}

void game::Collider::CreateRectangleColliderObject(
    Math::Vec2F position, Math::Vec2F rectMinBound, Math::Vec2F rectMaxBound,
    float mass, bool isTrigger, Physics::BodyType type) noexcept {
  CreateColliderObject(position, mass, isTrigger, Math::ShapeType::Rectangle,
                       type);
  auto& collider = world->GetCollider(colliderRef);
  auto& body = world->GetBody(bodyRef);
  collider.rectangleShape.SetMinBound(rectMinBound);
  collider.rectangleShape.SetMaxBound(rectMaxBound);
}

void game::Collider::RenderColliderObject() const noexcept {
  auto& curent_collider = world->GetCollider(colliderRef);
  auto& body = world->GetBody(bodyRef);
  switch (curent_collider._shape) {
    case Math::ShapeType::Rectangle:
      DrawRectangleLines(curent_collider.rectangleShape.MinBound().X,
                         curent_collider.rectangleShape.MinBound().Y,
                         curent_collider.rectangleShape.MaxBound().X -
                             curent_collider.rectangleShape.MinBound().X,
                         curent_collider.rectangleShape.MaxBound().Y -
                             curent_collider.rectangleShape.MinBound().Y,
                         color);
      break;
    case Math::ShapeType::Circle:
      DrawCircleLines(body.Position().X, body.Position().Y,
                      curent_collider.circleShape.Radius(), color);
      break;
    default:
      break;
  }
}

void game::Collider::UpdateCollider() const {
  auto& collider = world->GetCollider(colliderRef);
  const auto position = world->GetBody(bodyRef).Position();
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
