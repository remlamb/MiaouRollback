#include "PlayerManager.h"
#include <iostream>

PlayerManager::PlayerManager(Physics::World* world_) : world_(world_) {}

void PlayerManager::SetUp() {
	world_->contactListener = this;
	//trigger_nbrs_.fill(0);
	int it = 0;
	//TODO remplacer la variable ID dans la class par local
	for (auto& player : players) {
		Physics::BodyRef bodyRef = world_->CreateBody();
		auto& newBody = world_->GetBody(bodyRef);
		newBody.SetMass(1);
		if (it == 0)
		{
			newBody.SetPosition(player1_spawn_pos_);
		}
		else
		{
			newBody.SetPosition(player2_spawn_pos_);
		}

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
		groundedCollider.rectangleShape.SetMinBound({ 0.0f, 0.0f });
		groundedCollider.rectangleShape.SetMaxBound(grounded_collider_dimension_);
		players_grounded_CollidersRefs_[it] = colliderGroundedRef;
		it++;
	}
	Update();
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
		// groundedColliderObj.UpdateCollider();
		auto& groundedBody = world_->GetBody(players_grounded_BodyRefs_[it]);
		groundedBody.SetPosition(body.Position() -
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
		it++;
	}
}

void PlayerManager::Jump(int playerIdx) {
	auto& body = world_->GetBody(players_BodyRefs_[playerIdx]);
	if (players[playerIdx].is_grounded) {
		body.SetVelocity({ body.Velocity().X, jump_velocity_ });
	}
}
void PlayerManager::Move(bool rightDirection, int playerIdx) {
	auto& body = world_->GetBody(players_BodyRefs_[playerIdx]);
	float targetVelocityX = rightDirection ? move_velocity_ : -move_velocity_;
	body.SetVelocity(Math::Vec2F::Lerp(body.Velocity(),
		{ targetVelocityX, body.Velocity().Y },
		acceleration_time_));
}

void PlayerManager::Decelerate(int playerIdx) {
	auto& body = world_->GetBody(players_BodyRefs_[playerIdx]);
	const Math::Vec2F zeroVelocity = { 0.0f, body.Velocity().Y };
	body.SetVelocity(
		Math::Vec2F::Lerp(body.Velocity(), zeroVelocity, deceleration_time_));
}

Math::Vec2F PlayerManager::GetPlayerPosition(int idx) const noexcept {
	auto& body = world_->GetBody(players_BodyRefs_[idx]);
	return body.Position();
}


void PlayerManager::OnTriggerEnter(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {
	if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[0]) ||
		colliderB == world_->GetCollider(players_grounded_CollidersRefs_[0])) {
		players[0].trigger_nbr++;
	}

	if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[1]) ||
		colliderB == world_->GetCollider(players_grounded_CollidersRefs_[1])) {
		players[1].trigger_nbr++;
	}
}
void PlayerManager::OnTriggerExit(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {
	if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[0]) ||
		colliderB == world_->GetCollider(players_grounded_CollidersRefs_[0])) {
		players[0].trigger_nbr--;
	}

	if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[1]) ||
		colliderB == world_->GetCollider(players_grounded_CollidersRefs_[1])) {
		players[1].trigger_nbr--;
	}
}
void PlayerManager::OnCollisionEnter(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {}
void PlayerManager::OnCollisionExit(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {}
