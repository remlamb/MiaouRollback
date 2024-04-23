#include "GameApp.h"
#include "PhotonApp.h"

int main() {
  game::GameLogic game_logic;
  Renderer game_renderer{&game_logic};
  AudioManager audio_manager;

  GameApp game_app{&game_logic, &game_renderer, &audio_manager};

  game_app.Init();
  raylib::SetTargetFPS(60);
  while (!raylib::WindowShouldClose()) {
    game_app.Loop();
  }

  game_app.Deinit();

  return EXIT_SUCCESS;
}