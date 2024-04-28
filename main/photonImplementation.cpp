#include "GameApp.h"
#include "PhotonApp.h"

int main() {


  GameApp game_app;

  game_app.Init();
  raylib::SetTargetFPS(60);
  while (!raylib::WindowShouldClose()) {
    game_app.Loop();
  }

  game_app.Deinit();

  return EXIT_SUCCESS;
}