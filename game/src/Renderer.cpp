#include "Renderer.h"

#include <iostream>

void Renderer::Init() noexcept {
	icon.SetupIcon("data/weapon.png", Pivot::Center);
	SetWindowIcon(icon.icon);
	player.Setup("data/player1.png", 0.22f, Pivot::Center);
	player2.Setup("data/player2.png", 0.22f, Pivot::Center);
	playerWeapon.Setup("data/weapon.png", 1.0f, Pivot::Center);
	background.Setup("data/bg2.png", 1.0f, Pivot::Center);
	platform.Setup("data/platform.png", 0.10f, Pivot::Center);
	rope.Setup("data/rope.png", 1.0f, Pivot::Center);

	borderBottom.Setup("data/newbotBorder.png", 1.0f, Pivot::Default);
	borderLeft.Setup("data/TestBorder.png", 1.0f, Pivot::Default);
	borderRight.Setup("data/BorderRight.png", 1.0f, Pivot::Default);
	borderTop.Setup("data/newtopBorder.png", 1.0f, Pivot::Default);
}

void Renderer::Draw(bool isColliderVisible) noexcept {
	if (game_logic->current_game_state == game::GameState::LogMenu) {
		raylib::ClearBackground(raylib::Color{ 36, 77, 99, 1 });
		raylib::DrawRaylibText("Use the menu To Connect and Join a Game", 50, 0, 28,
			raylib::WHITE);
	}

	if (game_logic->current_game_state == game::GameState::GameLaunch) {
		raylib::ClearBackground(raylib::Color{ 36, 77, 99, 1 });
		if (isColliderVisible) {
			DrawColliderShape();
		}

		DrawProjectiles();
		DrawRopes();
		DrawLimit();
		DrawPlatforms();
		DrawPlayer();
	}

	if (game_logic->current_game_state == game::GameState::GameVictory)
	{
		raylib::ClearBackground(raylib::Color{ 36, 77, 99, 1 });
		if (game_logic->player_manager.players[game_logic->client_player_nbr].life_point <= 0)
		{
			raylib::DrawRaylibText("You loose", 50, 0, 28,
				raylib::WHITE);
		}
		else
		{
			raylib::DrawRaylibText("You Win", 50, 0, 28,
				raylib::WHITE);
		}

	}
}

void Renderer::Deinit() noexcept {
	player.TearDown();
	background.TearDown();
	// UnloadImage(icon);
}

void Renderer::DrawPlayerColliderShape() noexcept {
	int it = 0;
	for (auto player : game_logic->player_manager.players) {
		const auto& curent_collider = game_logic->world_.GetCollider(
			game_logic->player_manager.players_CollidersRefs_[it]);
		auto& body = game_logic->world_.GetBody(
			game_logic->player_manager.players_BodyRefs_[it]);
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

		auto& curent_collider2 = game_logic->world_.GetCollider(
			game_logic->player_manager.players_grounded_CollidersRefs_[it]);
		auto& body2 = game_logic->world_.GetBody(
			game_logic->player_manager.players_BodyRefs_[it]);
		auto color = raylib::PURPLE;
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

	for (const auto projectile : game_logic->player_manager.projectiles_)
	{
		auto collider = game_logic->world_.GetCollider(
			projectile.projectile_collider);
		auto body = game_logic->world_.GetBody(
			projectile.projectile_body);
		switch (collider._shape) {
		case Math::ShapeType::Rectangle:
			raylib::DrawRectangleLines(
				body.Position().X + collider.rectangleShape.MinBound().X,
				body.Position().Y + collider.rectangleShape.MinBound().Y,
				collider.rectangleShape.MaxBound().X -
				collider.rectangleShape.MinBound().X,
				collider.rectangleShape.MaxBound().Y -
				collider.rectangleShape.MinBound().Y,
				raylib::PURPLE);
			break;
		case Math::ShapeType::Circle:
			raylib::DrawCircleLines(body.Position().X, body.Position().Y,
				collider.circleShape.Radius(),
				raylib::PURPLE);
			break;
		default:
			break;
		}
	}
}

void Renderer::DrawColliderShape() noexcept {
	DrawPlayerColliderShape();
	for (auto& collider : game_logic->colliders_) {
		auto physicsCollider = game_logic->world_.GetCollider(collider.colliderRef);
		auto body = game_logic->world_.GetBody(collider.bodyRef);

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

void Renderer::DrawPlatforms() noexcept {
	platform.Draw(raylib::Vector2{ 250, game::GameLogic::screenHeight - 130 },
		1.4f);
	platform.Draw(raylib::Vector2{ game::GameLogic::screenWidth - 250,
								  game::GameLogic::screenHeight - 130 },
		1.4f);
	platform.Draw(raylib::Vector2{ game::GameLogic::screenWidth * 0.5,
								  game::GameLogic::screenHeight - 240 },
		1.4f);
}

void Renderer::DrawRopes() noexcept {
	rope.Draw(raylib::Vector2{ 450, 160 });
	rope.Draw(raylib::Vector2{ game::GameLogic::screenWidth - 450, 160 });
}

void Renderer::DrawPlayer() noexcept {
	raylib::Vector2 playerPosition =
		raylib::Vector2{ game_logic->player_manager.GetPlayerPosition(0).X,
						game_logic->player_manager.GetPlayerPosition(0).Y };

	if (game_logic->player_manager.players[0].is_projectile_ready)
	{
		playerWeapon.Draw({ playerPosition.x, playerPosition.y - 15 }, 0.2f);
	}
	player.Draw({ playerPosition.x, playerPosition.y - 15 });

	raylib::Vector2 player2Position =
		raylib::Vector2{ game_logic->player_manager.GetPlayerPosition(1).X,
						game_logic->player_manager.GetPlayerPosition(1).Y };

	if (game_logic->player_manager.players[1].is_projectile_ready)
	{
		playerWeapon.Draw({ player2Position.x, player2Position.y - 15 }, 0.2f);
	}
	player2.Draw({ player2Position.x, player2Position.y - 15 });
}

void Renderer::DrawProjectiles() noexcept
{
	for (const auto projectile : game_logic->player_manager.projectiles_)
	{
		auto body = game_logic->world_.GetBody(
			projectile.projectile_body);
		playerWeapon.Draw(Vector2{ body.Position().X, body.Position().Y }, 0.16f);
	}
}

void Renderer::DrawBackground() noexcept { background.Draw(center); }

void Renderer::DrawLimit() noexcept {
	borderBottom.Draw(raylib::Vector2{ 0, game::GameLogic::screenHeight - 20 });
	borderLeft.Draw(raylib::Vector2{ 0 - 20, 0 });
	borderRight.Draw(raylib::Vector2{ game::GameLogic::screenWidth - 60, 0 });
	borderTop.Draw(raylib::Vector2{ 0 - 5, 0 - 5 });
}
