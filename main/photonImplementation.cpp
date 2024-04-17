#include <imgui.h>

#include "PhotonApp.h"
#include "imgui_impl_raylib.h"
#include "raylib_wrapper.h"

int main() {
  PhotonApp app{};
  app.Setup();
#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
  while (!raylib::WindowShouldClose()) {
    raylib::ClearBackground(raylib::BLACK);
    app.Update();

    raylib::BeginDrawing();
    app.Draw();
    app.DrawImGui();
    raylib::EndDrawing();
  }

#endif

  app.TearDown();

  return EXIT_SUCCESS;
}