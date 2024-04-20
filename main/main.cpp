#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

#include "GameLogic.h"
#include "Renderer.h"

class App {
 public:
  game::GameLogic* game_logic;
  Renderer* game_renderer;

  raylib::Sound sound;
  raylib::Music music;

  App(game::GameLogic* gl, Renderer* renderer)
      : game_logic(gl), game_renderer(renderer) {}

  void Init() {
    raylib::InitAudioDevice();
    game_renderer->Init();
    sound = raylib::LoadSound("data/walkman.wav");
    music = raylib::LoadMusicStream("data/music.wav");
    music.looping = true;
    PlaySound(sound);

    PlayMusicStream(music);

    game_logic->Init();
  }

  void Deinit() {
    game_renderer->Deinit();
    UnloadSound(sound);  // Unload sound
    UnloadMusicStream(music);
    raylib::CloseAudioDevice();
    raylib::CloseWindow();

    game_logic->DeInit();
  }

  void Loop(void) {
    game_logic->Update();

    UpdateMusicStream(music);
    raylib::BeginDrawing();
    {
      raylib::ClearBackground(raylib::BLACK);
      game_renderer->Draw();
      // img.Draw(
      //     Vector2{game_logic.player.position.X,
      //     game_logic.player.position.Y}, 0.4f);

      //// DrawText("Coch-marre sample !!!", GameLogic::screenWidth / 2 - 200,
      //// 100,
      ////          30, WHITE);
    }
    raylib::EndDrawing();
  }
};

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) { static_cast<App*>(renderer)->Loop(); }

int main() {
  raylib::InitWindow(game::GameLogic::screenWidth,
                     game::GameLogic::screenHeight,
             "Online Game");
  game::GameLogic gameLogic;
  Renderer gameRenderer{&gameLogic};
  App app{&gameLogic, &gameRenderer};
  app.Init();

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
  raylib::SetTargetFPS(60);

  while (!raylib::WindowShouldClose()) {
    app.Loop();
  }

#endif

  app.Deinit();

  return EXIT_SUCCESS;
}
