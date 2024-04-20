#include <imgui.h>

#include "GameApp.h"
#include "GameLogic.h"
#include "PhotonApp.h"
#include "imgui_impl_raylib.h"
#include "raylib_wrapper.h"

int main() {
  game::GameLogic game_logic;
  Renderer game_renderer{&game_logic};
  //PhotonApp app{&game_logic};
  GameApp game_app{&game_logic, &game_renderer};

  game_app.Init();
  raylib::SetTargetFPS(60);

  while (!raylib::WindowShouldClose()) {
    // raylib::ClearBackground(raylib::BLACK);
    // app.Update();

    // raylib::BeginDrawing();
    // app.Draw();
    // app.DrawImGui();
    // raylib::EndDrawing();

    game_app.Loop();
  }

  // app.TearDown();

  return EXIT_SUCCESS;
}