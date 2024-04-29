#include "PlayerManager.h"
#include <iostream>

PlayerManager::PlayerManager(Physics::World* world_) : world_(world_) {}

void PlayerManager::SetUp() {
	world_->contactListener = this;
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
		if (it == 0)
		{
			newCollider.ID = player1_collider_id_;
		}
		else
		{
			newCollider.ID = player2_collider_id_;
		}



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
		if (it == 0)
		{
			groundedCollider.ID = player1_groundedcollider_id_;
		}
		else
		{
			groundedCollider.ID = player2_groundedcollider_id_;
		}
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

	for (auto& projectile : projectiles_)
	{
		auto& collider = world_->GetCollider(projectile.projectile_collider);
		const auto position = world_->GetBody(projectile.projectile_body).Position();
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

void PlayerManager::Attack(int player_idx)
{
	if(players[player_idx].attack_timer <= 0.0f)
	{
		players[player_idx].attack_timer = time_between_attack;
		players[player_idx].is_projectile_ready = false;

		//create a new world collider for the projectile
		Physics::BodyRef bodyRef = world_->CreateBody();
		auto& newBody = world_->GetBody(bodyRef);
		newBody.SetMass(1);
		newBody.SetPosition(GetPlayerPosition(player_idx));
		newBody.SetVelocity(projectile_speed_);
		newBody.type = Physics::BodyType::DYNAMIC;

		Physics::ColliderRef colliderRef = world_->CreateCollider(bodyRef);
		auto& newCollider = world_->GetCollider(colliderRef);
		newCollider._shape = Math::ShapeType::Circle;
		newCollider.isTrigger = true;
		newCollider.restitution = 0.0f;
		newCollider.circleShape.SetRadius(projectile_radius_);
		newCollider.ID = current_projectile_collider_id_++;
		auto projectile = Projectile{ bodyRef , colliderRef };
		projectile.nbr_launching_player = player_idx;
		projectiles_.emplace_back(projectile);
	}
	
}


void PlayerManager::OnTriggerEnter(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {
	int it = 0;
	for (auto& player : players)
	{
		if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[it]) ||
			colliderB == world_->GetCollider(players_grounded_CollidersRefs_[it])) {
			if (colliderA.ID >= projectile_id_ || colliderB.ID >= projectile_id_)
			{
				break;
			}
			if (colliderA.ID == neutral_projectile_id_ || colliderB.ID == neutral_projectile_id_)
			{
				break;
			}
			players[it].trigger_nbr++;
		}
		it++;
	}


	for (auto& projectile : projectiles_)
	{
		if (colliderA == world_->GetCollider(projectile.projectile_collider) ||
			colliderB == world_->GetCollider(projectile.projectile_collider)) {
			//todo better code to stop the projectile only in contact of platform
			if(colliderA.ID == 10 || colliderB.ID == 10)
			{
				auto& body = world_->GetBody(projectile.projectile_body);
				body.SetVelocity(Math::Vec2F(0, 0));
				auto& projectile_collider = world_->GetCollider(projectile.projectile_collider);
				projectile_collider.ID = neutral_projectile_id_;
			}

			if(colliderA.ID == player1_collider_id_ || colliderB.ID == player1_collider_id_)
			{
				if(projectile.nbr_launching_player != 0)
				{
					std::cout << "Touched ! " << std::endl;
				}

			}
			if (colliderA.ID == player2_collider_id_ || colliderB.ID == player2_collider_id_)
			{
				if (projectile.nbr_launching_player != 1)
				{
					std::cout << "Touched ! " << std::endl;
				}
			}


			//si neutral et player 1 ou player 2
			if ((colliderA.ID == neutral_projectile_id_ || colliderB.ID == neutral_projectile_id_) && (colliderA.ID == player1_collider_id_ || colliderB.ID == player1_collider_id_))
			{
				std::cout << "Touched ! " << std::endl;
			}

			if ((colliderA.ID == neutral_projectile_id_ || colliderB.ID == neutral_projectile_id_) && (colliderA.ID == player2_collider_id_ || colliderB.ID == player2_collider_id_))
			{
				std::cout << "Touched ! " << std::endl;
			}
		}
	}
}
void PlayerManager::OnTriggerExit(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {
	int it = 0;
	for (auto& player : players)
	{
		if (colliderA == world_->GetCollider(players_grounded_CollidersRefs_[it]) ||
			colliderB == world_->GetCollider(players_grounded_CollidersRefs_[it])) {
			if (colliderA.ID >= projectile_id_ || colliderB.ID >= projectile_id_)
			{
				break;
			}
			if (colliderA.ID == neutral_projectile_id_ || colliderB.ID == neutral_projectile_id_)
			{
				break;
			}
			players[it].trigger_nbr--;
		}
		it++;
	}
}
void PlayerManager::OnCollisionEnter(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {}
void PlayerManager::OnCollisionExit(Physics::Collider colliderA,
	Physics::Collider colliderB) noexcept {}
