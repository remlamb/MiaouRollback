#include "GameLogic.h"

#include <iostream>

#include "Random.h"

void GameLogic::Init() noexcept
{
	timer_.OnStart();
	world_.Init();

	world_.contactListener = this;

	CreateRectangleColliderObject({ 0, 700 }, { 0.0, 0.0 }, { 1080.0, 20.0 }, 1, false, Engine::BodyType::STATIC);
	CreateRectangleColliderObject({ 1060, 0 }, { 0.0, 0.0 }, { 20.0, 720.0 }, 1, false, Engine::BodyType::STATIC);
	CreateRectangleColliderObject({ 0, 0 }, { 0.0, 0.0 }, { 1080.0, 20.0 }, 1, false, Engine::BodyType::STATIC);
	CreateRectangleColliderObject({ 0, 0 }, { 0.0, 0.0 }, { 20.0, 720.0 }, 1, false, Engine::BodyType::STATIC);



	for (int i = 0; i < 50; i++)
	{
		CreateCircleColliderObject({ 250 + static_cast<float>(i * 2), 650 + static_cast<float>(i * 2.5) }, 20.0f, 100, false, Engine::BodyType::DYNAMIC);
		auto& ccollider = world_.GetCollider(Colliders[colliderCurrentID - 1].colliderRef);
		auto& cbody = world_.GetBody(Colliders[colliderCurrentID - 1].bodyRef);
		cbody.SetVelocity(Math::Vec2F(80.0f, 120.0f));
		ccollider.restitution = 1.0f;
	}

	//player

	//CreateRectangleColliderObject({ 50 , 650 }, { 0.0, 0.0 }, { 20.0, 20.0 }, 100, false, Engine::BodyType::DYNAMIC);
	CreateCircleColliderObject({ 50 , 650 }, 10.0, 100, false, Engine::BodyType::DYNAMIC);

	player.collider = Colliders[colliderCurrentID - 1];
	auto& collider = world_.GetCollider(player.collider.colliderRef);
	auto& body = world_.GetBody(player.collider.bodyRef);
	body.SetVelocity(Math::Vec2F(0.0f, 10.0f));
	collider.restitution = 0.0f;

}
void GameLogic::Update() noexcept
{
	for (auto& circle : Colliders)
	{
		auto& collider = world_.GetCollider(circle.colliderRef);
		const auto position = world_.GetBody(circle.bodyRef).Position();
		switch (collider._shape)
		{
		case Math::ShapeType::Rectangle:
			collider.rectangleShape = Math::RectangleF(position, position + collider.rectangleShape.MaxBound() - collider.rectangleShape.MinBound());
			break;
		case Math::ShapeType::Circle:
			collider.circleShape = Math::CircleF(position, collider.circleShape.Radius());
			break;
		default:
			break;
		}
	}

	world_.Update(timer_.DeltaTime());
}
void GameLogic::DeInit() noexcept
{
	_bodyRefs.clear();
	_colRefs.clear();
	world_.Clear();

	world_.contactListener = nullptr;
	Colliders.empty();
}

void GameLogic::Jump()
{
	auto& body = world_.GetBody(player.collider.bodyRef);
	body.SetVelocity(Math::Vec2F(0.0f, -100.0f));
}

ColliderObject GameLogic::CreateColliderObject(Math::Vec2F position, float mass, bool isTrigger, Math::ShapeType shape, Engine::BodyType type) noexcept
{
	ColliderObject newColliderObject;
	newColliderObject.bodyRef = world_.CreateBody();
	auto& newBody = world_.GetBody(newColliderObject.bodyRef);
	newBody.SetMass(mass);
	newBody.SetPosition(position);
	newBody.SetVelocity(Math::Vec2F(0, 0));
	newBody.type = type;
	newColliderObject.colliderRef = world_.CreateCollider(newColliderObject.bodyRef);
	auto& newCollider = world_.GetCollider(newColliderObject.colliderRef);
	newCollider.ID = colliderCurrentID++;
	newCollider._shape = shape;
	newCollider.isTrigger = isTrigger;
	newCollider.restitution = 0.0f;
	switch (newBody.type)
	{
	case Engine::BodyType::DYNAMIC:
		if (newCollider.isTrigger)
		{
			newColliderObject.color = BLUE;
		}
		else
		{
			newColliderObject.color = RED;
		}
		break;
	case Engine::BodyType::STATIC:
		if (newCollider.isTrigger)
		{
			newColliderObject.color = ORANGE;
		}
		else
		{
			newColliderObject.color = YELLOW;
		}
		break;
	default:
		break;
	}
	const auto bodyPosition = newBody.Position();
	Colliders.emplace_back(newColliderObject);
	return newColliderObject;
}
void GameLogic::CreateCircleColliderObject(Math::Vec2F position, float radius, float mass, bool isTrigger, Engine::BodyType type) noexcept
{
	ColliderObject circle = CreateColliderObject(position, mass, isTrigger, Math::ShapeType::Circle, type);
	auto& collider = world_.GetCollider(circle.colliderRef);
	auto& body = world_.GetBody(circle.bodyRef);
	collider.circleShape.SetRadius(radius);
}
void GameLogic::CreateRectangleColliderObject(Math::Vec2F position, Math::Vec2F rectMinBound, Math::Vec2F rectMaxBound, float mass, bool isTrigger, Engine::BodyType type) noexcept
{
	ColliderObject Rect = CreateColliderObject(position, mass, isTrigger, Math::ShapeType::Rectangle, type);
	auto& collider = world_.GetCollider(Rect.colliderRef);
	auto& body = world_.GetBody(Rect.bodyRef);
	collider.rectangleShape.SetMinBound(rectMinBound);
	collider.rectangleShape.SetMaxBound(rectMaxBound);
}


void GameLogic::RenderColliderObject() noexcept
{
	for (auto& collider : Colliders)
	{
		auto& curent_collider = world_.GetCollider(collider.colliderRef);
		auto& body = world_.GetBody(collider.bodyRef);
		switch (curent_collider._shape)
		{
		case Math::ShapeType::Rectangle:
			DrawRectangleLines(curent_collider.rectangleShape.MinBound().X, curent_collider.rectangleShape.MinBound().Y,
				curent_collider.rectangleShape.MaxBound().X - curent_collider.rectangleShape.MinBound().X,
				curent_collider.rectangleShape.MaxBound().Y - curent_collider.rectangleShape.MinBound().Y,
				collider.color);
			break;
		case Math::ShapeType::Circle:
			DrawCircleLines(body.Position().X, body.Position().Y, curent_collider.circleShape.Radius(),
				collider.color);
			break;
		default:
			break;
		}
	}
}

void GameLogic::OnTriggerEnter(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{
	for (auto& collider : Colliders)
	{
		const auto currentCollider = world_.GetCollider(collider.colliderRef);
		if (currentCollider == colliderA || currentCollider == colliderB)
		{
			collider.CollisionNbr = true;
		}
	}
}
void GameLogic::OnTriggerExit(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{
	for (auto& collider : Colliders)
	{
		const auto currentCollider = world_.GetCollider(collider.colliderRef);
		if (currentCollider == colliderA || currentCollider == colliderB)
		{
			collider.CollisionNbr = false;
		}
	}
}
void GameLogic::OnCollisionEnter(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{

}
void GameLogic::OnCollisionExit(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{

}