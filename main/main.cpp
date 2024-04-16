#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

#include "GameLogic.h"
#include "Renderer.h"

class App {
 public:
  GameLogic* game_logic;
  Renderer* game_renderer;

  Sound sound;
  Music music;

  

  App(GameLogic* gl, Renderer* renderer) : game_logic(gl), game_renderer(renderer){}

  void Init() {
    InitAudioDevice();
    game_renderer->Init();
    sound = LoadSound("data/walkman.wav");
    music = LoadMusicStream("data/music.wav");
    music.looping = true;
    PlaySound(sound);

    PlayMusicStream(music);

    game_logic->Init();
  }

  void Deinit() {
    game_renderer->Deinit();
    UnloadSound(sound);  // Unload sound
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    game_logic->DeInit();
  }

  void Loop(void) {
    game_logic->Update();

    UpdateMusicStream(music);
    BeginDrawing();
    {
      ClearBackground(BLACK);
      game_renderer->Draw();
      // img.Draw(
      //     Vector2{game_logic.player.position.X,
      //     game_logic.player.position.Y}, 0.4f);

      //// DrawText("Coch-marre sample !!!", GameLogic::screenWidth / 2 - 200,
      //// 100,
      ////          30, WHITE);
    }
    EndDrawing();


  }
};

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) { static_cast<App*>(renderer)->Loop(); }

int main() {
  InitWindow(GameLogic::screenWidth, GameLogic::screenHeight, "Online Game");
  GameLogic gameLogic;
  Renderer gameRenderer{&gameLogic};
  App app{&gameLogic, &gameRenderer};
  app.Init();

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    app.Loop();
  }

#endif

  app.Deinit();

  return EXIT_SUCCESS;
}
