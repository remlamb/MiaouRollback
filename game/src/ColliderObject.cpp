#include "ColliderObject.h"

void ColliderObject::CreateColliderObject(Math::Vec2F position, float mass,
                                          bool isTrigger, Math::ShapeType shape,
                                          Engine::BodyType type) noexcept {
  bodyRef = world.CreateBody();
  auto& newBody = world.GetBody(bodyRef);
  newBody.SetMass(mass);
  newBody.SetPosition(position);
  newBody.SetVelocity(Math::Vec2F(0, 0));
  newBody.type = type;
  colliderRef = world.CreateCollider(bodyRef);
  auto& newCollider = world.GetCollider(colliderRef);
  newCollider._shape = shape;
  newCollider.isTrigger = isTrigger;
  newCollider.restitution = 0.0f;
  switch (newBody.type) {
    case Engine::BodyType::DYNAMIC:
      if (newCollider.isTrigger) {
        color = BLUE;
      } else {
        color = RED;
      }
      break;
    case Engine::BodyType::STATIC:
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

void ColliderObject::CreateCircleColliderObject(
    Math::Vec2F position, float radius, float mass, bool isTrigger,
    Engine::BodyType type) noexcept {
  CreateColliderObject(position, mass, isTrigger, Math::ShapeType::Circle,
                       type);
  auto& collider = world.GetCollider(colliderRef);
  auto& body = world.GetBody(bodyRef);
  collider.circleShape.SetRadius(radius);
}

void ColliderObject::CreateRectangleColliderObject(
    Math::Vec2F position, Math::Vec2F rectMinBound, Math::Vec2F rectMaxBound,
    float mass, bool isTrigger, Engine::BodyType type) noexcept {
  CreateColliderObject(position, mass, isTrigger, Math::ShapeType::Rectangle,
                       type);
  auto& collider = world.GetCollider(colliderRef);
  auto& body = world.GetBody(bodyRef);
  collider.rectangleShape.SetMinBound(rectMinBound);
  collider.rectangleShape.SetMaxBound(rectMaxBound);
}

void ColliderObject::RenderColliderObject() const noexcept {
  auto& curent_collider = world.GetCollider(colliderRef);
  auto& body = world.GetBody(bodyRef);
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