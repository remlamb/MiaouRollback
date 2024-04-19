#include "GameLogic.h"

#include <iostream>

#include "Random.h"

namespace game
{
	void GameLogic::Init() noexcept {
		timer_.OnStart();
		world_.Init();
		player.SetUp();

		//// Border
		CreatePlatform({ 0, 0 }, { 0.0, 0.0 }, { screenWidth, border_size_ });
		CreatePlatform({ 0, 0 }, { 0.0, 0.0 }, { border_size_, screenHeight });
		CreatePlatform({ 0, screenHeight - border_size_ }, { 0,0 }, { screenWidth, border_size_ });
		CreatePlatform({ screenWidth - border_size_, 0 }, { 0,0 }, { border_size_, screenHeight });

		////Platform
		CreatePlatform({ 250 - static_cast<float>(platform_size_.X * 0.5), screenHeight - 150 }, { 0.0, 0.0 }, { platform_size_ });
		CreatePlatform({ screenWidth - 250 - static_cast<float>(platform_size_.X * 0.5),screenHeight - 150 }, { 0.0, 0.0 }, { platform_size_ });
		CreatePlatform({ static_cast<float>(screenWidth * 0.5) - static_cast<float>(platform_size_.X * 0.5),static_cast<float>(platform_size_.X * 0.5) + screenHeight * 0.5f }, { 0,0 }, { platform_size_ });

		//// Rope
		CreateRope({ 450 - static_cast<float>(20.0f * 0.5), screenHeight - 650 }, { 0.0, 0.0 }, { 20.0f, 250.0f });
		CreateRope({ screenWidth - 450 - static_cast<float>(20.0f * 0.5), screenHeight - 650 }, { 0.0, 0.0 }, { 20.0f, 250.0f });
	}

	void GameLogic::Update() noexcept {
		// PlayerManager Input
		ManageInput();
		UpdateCollider();

		player.Update();
		world_.Update(1 / 50.0f);
	}

	void GameLogic::DeInit() noexcept {
		world_.Clear();

		world_.contactListener = nullptr;
		colliders_.empty();
	}

	void GameLogic::ManageInput() noexcept {
		inputs.SetPlayerInputs();
		if (inputs.playerInput & static_cast<std::uint8_t>(Input::kJump)) {
			player.Jump(0);
		}
		if (inputs.playerInput & static_cast<std::uint8_t>(Input::kLeft)) {
			player.Move(false, 0);
		}
		else if (inputs.playerInput & static_cast<std::uint8_t>(Input::kRight)) {
			player.Move(true, 0);
		}
		else {
			player.Decelerate(0);
		}

		//Todo Remove Temporary for local
		if (IsKeyDown(KEY_UP)) {
			player.Jump(1);
		}
		if (IsKeyDown(KEY_LEFT)) {
			player.Move(false, 1);
		}
		else if (IsKeyDown(KEY_RIGHT)) {
			player.Move(true, 1);
		}
		else {
			player.Decelerate(1);
		}

	}

	void GameLogic::CreatePlatform(Math::Vec2F position, Math::Vec2F rectMinBound,
		Math::Vec2F rectMaxBound) noexcept
	{
		Physics::BodyRef bodyRef = world_.CreateBody();
		auto& newBody = world_.GetBody(bodyRef);
		newBody.SetMass(1);
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
		colliders_.emplace_back(collider{ bodyRef, colliderRef });
	}

	void GameLogic::CreateRope(Math::Vec2F position, Math::Vec2F rectMinBound,
		Math::Vec2F rectMaxBound) noexcept
	{
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
		colliders_.emplace_back(collider{ bodyRef, colliderRef });
	}


	void GameLogic::UpdateCollider() noexcept {
		for (auto& collider : colliders_)
		{
			auto& physicsCollider = world_.GetCollider(collider.colliderRef);
			auto& physicsBody = world_.GetBody(collider.bodyRef);
			switch (physicsCollider._shape) {
			case Math::ShapeType::Rectangle:
				physicsCollider.rectangleShape = Math::RectangleF(
					physicsBody.Position(), physicsBody.Position() + physicsCollider.rectangleShape.MaxBound() -
					physicsCollider.rectangleShape.MinBound());
				break;
			case Math::ShapeType::Circle:
				physicsCollider.circleShape =
					Math::CircleF(physicsBody.Position(), physicsCollider.circleShape.Radius());
				break;
			default:
				break;
			}
		}
	}


	void GameLogic::RenderColliderObject() noexcept {
		for (auto& collider : colliders_)
		{
			auto physicsCollider = world_.GetCollider(collider.colliderRef);
			auto body = world_.GetBody(collider.bodyRef);

			Color color = WHITE;
			switch (body.type) {
			case Physics::BodyType::DYNAMIC:
				if (physicsCollider.isTrigger) {
					color = BLUE;
				}
				else {
					color = RED;
				}
				break;
			case Physics::BodyType::STATIC:
				if (physicsCollider.isTrigger) {
					color = ORANGE;
				}
				else {
					color = YELLOW;
				}
				break;
			default:
				break;
			}

			switch (physicsCollider._shape) {
			case Math::ShapeType::Rectangle:
				DrawRectangleLines(physicsCollider.rectangleShape.MinBound().X,
					physicsCollider.rectangleShape.MinBound().Y,
					physicsCollider.rectangleShape.MaxBound().X -
					physicsCollider.rectangleShape.MinBound().X,
					physicsCollider.rectangleShape.MaxBound().Y -
					physicsCollider.rectangleShape.MinBound().Y,
					color);

				break;
			case Math::ShapeType::Circle:
				DrawCircleLines(body.Position().X, body.Position().Y,
					physicsCollider.circleShape.Radius(), color);
				break;
			default:
				break;
			}
		}
	}
}

