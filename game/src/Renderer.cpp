#include "Renderer.h"

void Renderer::Init() noexcept {
  icon = LoadImage("data/weapon.png");
  SetWindowIcon(icon);
  img.Setup("data/2222.png", 1.0f, Pivot::Center);
  player.Setup("data/player.png", 1.0f, Pivot::Center);
  playerWeapon.Setup("data/weapon.png", 1.0f, Pivot::Center);
  background.Setup("data/bg.png", 1.0f, Pivot::Center);
  platform.Setup("data/platform.png", 1.0f, Pivot::Center);

  borderBottom.Setup("data/botBorder.png", 1.0f, Pivot::Default);
  borderLeft.Setup("data/leftBorder.png", 1.0f, Pivot::Default);
  borderRight.Setup("data/rightBorder.png", 1.0f, Pivot::Default);
  borderTop.Setup("data/topBorder.png", 1.0f, Pivot::Default);
}

void Renderer::Draw() noexcept {
  customScale += 0.01f;
  DrawBackground();
  DrawColliderShape();
  img.Draw(
      Vector2{GameLogic::screenWidth * 0.8f, GameLogic::screenHeight * 0.5f});

  DrawLimit();
  DrawPlatforms();
  DrawPlayer();
}

void Renderer::Deinit() noexcept {
  img.TearDown();
  player.TearDown();
  background.TearDown();
  UnloadImage(icon);
}

void Renderer::DrawColliderShape() noexcept
{
  for (auto collider : game_logic->Colliders) {
    collider.RenderColliderObject();
  }
  game_logic->player.DrawDebug();
}

void Renderer::DrawPlatforms() noexcept {
  platform.Draw(Vector2{250, GameLogic::screenHeight - 130}, 1.4f);
  platform.Draw(
      Vector2{GameLogic::screenWidth - 250, GameLogic::screenHeight - 130},
      1.4f);
  platform.Draw(
      Vector2{GameLogic::screenWidth * 0.5, GameLogic::screenHeight - 280},
      1.4f);
}

void Renderer::DrawPlayer() noexcept {
  Vector2 playerPosition = Vector2{game_logic->player.players_[0].position.X,
                                   game_logic->player.players_[0].position.Y};

  playerWeapon.Draw(playerPosition, 0.2f);
  player.Draw(playerPosition, 0.4f);
}

void Renderer::DrawBackground() noexcept { background.Draw(center); }

void Renderer::DrawLimit() noexcept {
  borderBottom.Draw(Vector2{0, GameLogic::screenHeight - 20});
  borderLeft.Draw(Vector2{0 - 50, 0});
  borderRight.Draw(Vector2{GameLogic::screenWidth - 20, 0});
  borderTop.Draw(Vector2{0, 0 - 50});
}
