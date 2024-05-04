#include "Renderer.h"

#include <iostream>

#include "NetworkLogic.h"

void Renderer::Init() noexcept {
  icon_.SetupIcon("data/weapon.png", Pivot::Center);
  SetWindowIcon(icon_.icon);
  player_.Setup("data/player1.png", 0.22f, Pivot::Center);
  player2_.Setup("data/player2.png", 0.22f, Pivot::Center);
  player_weapon_.Setup("data/weapon.png", 1.0f, Pivot::Center);
  background_.Setup("data/bg.png", 1.0f, Pivot::Center);
  platform_.Setup("data/platformv2.png", 0.10f, Pivot::Center);
  rope_.Setup("data/rope.png", 1.0f, Pivot::Center);

  border_bottom_.Setup("data/newbotBorder.png", 1.0f, Pivot::Default);
  border_left_.Setup("data/TestBorder.png", 1.0f, Pivot::Default);
  border_right_.Setup("data/BorderRight.png", 1.0f, Pivot::Default);
  border_top_.Setup("data/newtopBorder.png", 1.0f, Pivot::Default);
  main_menu_bg_.Setup("data/main_menu.png", 1.0f, Pivot::Center);
  winner_layer_p1.Setup("data/winner_layer_p1.png", 1.0f, Pivot::Center);
  winner_layer_p2.Setup("data/winner_layer_p2.png", 1.0f, Pivot::Center);

  player1_life_point_.Setup("data/P1LP.png", 1.0f, Pivot::Center);
  player2_life_point_.Setup("data/P2LP.png", 1.0f, Pivot::Center);
}

void Renderer::Draw(bool isColliderVisible) noexcept {
  if (game_logic_->current_game_state == game::GameState::LogMenu) {
    raylib::ClearBackground(raylib::Color{20, 20, 20, 1});
    main_menu_bg_.Draw(center_pos_);
    raylib::DrawRaylibText(game::game_name, 50, 40, 28, raylib::WHITE);
    raylib::DrawRaylibText("Log:", 500, 500, 12, raylib::WHITE);
    raylib::DrawRaylibText(network_logic_->currentLogInfo, 500, 520, 12,
                           raylib::WHITE);
  }

  if (game_logic_->current_game_state == game::GameState::GameLaunch) {
    DrawBackground();
    raylib::ClearBackground(raylib::Color{36, 77, 99, 1});
    if (isColliderVisible) {
      DrawColliderShape();
    }

    DrawProjectiles();
    DrawRopes();
    DrawLimit();
    DrawPlatforms();
    DrawPlayer();
    DrawUI();
  }

  if (game_logic_->current_game_state == game::GameState::GameVictory) {
    raylib::ClearBackground(raylib::Color{36, 77, 99, 1});
    background_.Draw(center_pos_);

    if (game_logic_->player_manager.players[0]
            .life_point > 0) {
      winner_layer_p1.Draw(center_pos_);
    } else {
      winner_layer_p2.Draw(center_pos_);
    }


    if (game_logic_->player_manager.players[game_logic_->client_player_nbr]
            .life_point <= 0) {
      raylib::DrawRaylibText("Maybe Next Time?", 50, 40, 28, raylib::WHITE);
    } else {
      raylib::DrawRaylibText("Congratulation", 50, 40, 28, raylib::WHITE);
    }
  }
}

void Renderer::Deinit() noexcept {
  player_.TearDown();
  background_.TearDown();
}

void Renderer::DrawPlayerColliderShape() noexcept {
  int it = 0;
  for (auto player : game_logic_->player_manager.players) {
    const auto& curent_collider = game_logic_->world_.GetCollider(
        game_logic_->player_manager.players_CollidersRefs_[it]);
    auto& body = game_logic_->world_.GetBody(
        game_logic_->player_manager.players_BodyRefs_[it]);
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

    auto& curent_collider2 = game_logic_->world_.GetCollider(
        game_logic_->player_manager.players_grounded_CollidersRefs_[it]);
    auto& body2 = game_logic_->world_.GetBody(
        game_logic_->player_manager.players_BodyRefs_[it]);
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

  for (const auto projectile : game_logic_->player_manager.projectiles_) {
    auto collider =
        game_logic_->world_.GetCollider(projectile.projectile_collider);
    auto body = game_logic_->world_.GetBody(projectile.projectile_body);
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
                                collider.circleShape.Radius(), raylib::PURPLE);
        break;
      default:
        break;
    }
  }
}

void Renderer::DrawColliderShape() noexcept {
  DrawPlayerColliderShape();
  for (auto& collider : game_logic_->colliders_) {
    auto physicsCollider =
        game_logic_->world_.GetCollider(collider.colliderRef);
    auto body = game_logic_->world_.GetBody(collider.bodyRef);

    Color color = WHITE;
    switch (body.type) {
      case Physics::BodyType::DYNAMIC:
        if (physicsCollider.isTrigger) {
          color = BLUE;
        } else {
          color = RED;
        }
        break;
      case Physics::BodyType::STATIC:
        if (physicsCollider.isTrigger) {
          color = ORANGE;
        } else {
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
  platform_.Draw(raylib::Vector2{250, game::screen_height - 130}, 1.4f);
  platform_.Draw(
      raylib::Vector2{game::screen_width - 250, game::screen_height - 130},
      1.4f);
  platform_.Draw(
      raylib::Vector2{game::screen_width * 0.5, game::screen_height - 230},
      1.4f);
}

void Renderer::DrawRopes() noexcept {
  rope_.Draw(raylib::Vector2{450, 160});
  rope_.Draw(raylib::Vector2{game::screen_width - 450, 160});
}

void Renderer::DrawPlayer() noexcept {
  raylib::Vector2 playerPosition =
      raylib::Vector2{game_logic_->player_manager.GetPlayerPosition(0).X,
                      game_logic_->player_manager.GetPlayerPosition(0).Y};

  if (game_logic_->player_manager.players[0].is_projectile_ready) {
    player_weapon_.Draw({playerPosition.x, playerPosition.y - 15}, 0.2f);
  }
  player_.Draw({playerPosition.x, playerPosition.y - 15});

  raylib::Vector2 player2Position =
      raylib::Vector2{game_logic_->player_manager.GetPlayerPosition(1).X,
                      game_logic_->player_manager.GetPlayerPosition(1).Y};

  if (game_logic_->player_manager.players[1].is_projectile_ready) {
    player_weapon_.Draw({player2Position.x, player2Position.y - 15}, 0.2f);
  }
  player2_.Draw({player2Position.x, player2Position.y - 15});
}

void Renderer::DrawProjectiles() noexcept {
  for (const auto projectile : game_logic_->player_manager.old_projectiles_) {
    auto body = game_logic_->world_.GetBody(projectile.projectile_body);
    player_weapon_.Draw(Vector2{body.Position().X, body.Position().Y}, 0.16f);
  }

  for (int i = 0; i < game_logic_->player_manager.max_projectile_; i++) {
    auto pos =
        raylib::Vector2{game_logic_->player_manager.GetProjectilePosition(i).X,
                        game_logic_->player_manager.GetProjectilePosition(i).Y};
    player_weapon_.Draw(pos, 0.16f);
  }
}

void Renderer::DrawBackground() noexcept { background_.Draw(center_pos_); }

void Renderer::DrawLimit() noexcept {
  border_bottom_.Draw(raylib::Vector2{0, game::screen_height - 20});
  border_left_.Draw(raylib::Vector2{0 - 20, 0});
  border_right_.Draw(raylib::Vector2{game::screen_width - 60, 0});
  border_top_.Draw(raylib::Vector2{0 - 5, 0 - 5});
}

void Renderer::DrawUI() noexcept {

  for (int i = game_logic_->player_manager.players[0].life_point; i > 0; i--) {
    player1_life_point_.Draw(raylib::Vector2{static_cast<float>(50 * i), 40},
                             0.2f);
  }

  for (int i = game_logic_->player_manager.players[1].life_point; i > 0; i--) {
    player2_life_point_.Draw(
        raylib::Vector2{static_cast<float>(game::screen_width - 50 * i), 40},
        0.2f);
  }
}
