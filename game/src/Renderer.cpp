#include "Renderer.h"

void Renderer::Init() noexcept {
  icon = raylib::LoadImage("data/weapon.png");
  SetWindowIcon(icon);
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

void Renderer::Draw() noexcept {
    if (game_logic->current_game_state == game::GameState::LogMenu)
    {
        raylib::ClearBackground(raylib::Color{ 36, 77, 99, 1 });
        raylib::DrawRaylibText("Use the menu To Connect and Join a Game", 50, 0, 28,
            raylib::WHITE);
    }

    if(game_logic->current_game_state == game::GameState::GameLaunch)
    {
        // DrawBackground();
        raylib::ClearBackground(raylib::Color{ 36, 77, 99, 1 });
        //DrawColliderShape();

        DrawRopes();
        DrawLimit();
        DrawPlatforms();
        DrawPlayer();
    }
}

void Renderer::Deinit() noexcept {
  player.TearDown();
  background.TearDown();
  UnloadImage(icon);
}

void Renderer::DrawColliderShape() noexcept {
  game_logic->RenderColliderObject();
  game_logic->player.DrawDebug();
}

void Renderer::DrawPlatforms() noexcept {
  platform.Draw(raylib::Vector2{250, game::GameLogic::screenHeight - 130},
                1.4f);
  platform.Draw(raylib::Vector2{game::GameLogic::screenWidth - 250,
                        game::GameLogic::screenHeight - 130},
                1.4f);
  platform.Draw(raylib::Vector2{game::GameLogic::screenWidth * 0.5,
                        game::GameLogic::screenHeight - 240},
                1.4f);
}

void Renderer::DrawRopes() noexcept {
  rope.Draw(raylib::Vector2{450, 160});
  rope.Draw(raylib::Vector2{game::GameLogic::screenWidth - 450, 160});
}

void Renderer::DrawPlayer() noexcept {
  raylib::Vector2 playerPosition =
      raylib::Vector2{game_logic->player.players[0].position.X,
                                   game_logic->player.players[0].position.Y};

  playerWeapon.Draw({playerPosition.x, playerPosition.y - 15}, 0.2f);
  player.Draw({playerPosition.x, playerPosition.y - 15});

  raylib::Vector2 player2Position =
      raylib::Vector2{game_logic->player.players[1].position.X,
                                    game_logic->player.players[1].position.Y};

  playerWeapon.Draw({player2Position.x, player2Position.y - 15}, 0.2f);
  player2.Draw({player2Position.x, player2Position.y - 15});
}

void Renderer::DrawBackground() noexcept { background.Draw(center); }

void Renderer::DrawLimit() noexcept {
  borderBottom.Draw(raylib::Vector2{0, game::GameLogic::screenHeight - 20});
  borderLeft.Draw(raylib::Vector2{0 - 20, 0});
  borderRight.Draw(raylib::Vector2{game::GameLogic::screenWidth - 60, 0});
  borderTop.Draw(raylib::Vector2{0 - 5, 0 - 5});
}
