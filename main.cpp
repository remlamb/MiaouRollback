#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

#include "GameLogic.h"
#include "Image.h"

class Renderer {
 public:
  GameLogic game_logic;
  Sound sound;
  Music music;

  ImageCustom img;
  ImageCustom omniman;
  Image icon;
  float customScale = 0.0f;
  Vector2 center = {GameLogic::screenWidth * 0.5f,
                    GameLogic::screenHeight * 0.5f};

  void Init() {
    icon = LoadImage("data/weapon.png");
    SetWindowIcon(icon);

    img.Setup("data/2222.png", 1.0f, Pivot::Center);
    omniman.Setup("data/omniman.png", 1.8f, Pivot::Center);
    InitAudioDevice();
    sound = LoadSound("data/walkman.wav");
    music = LoadMusicStream("data/music.wav");
    music.looping = true;
    PlaySound(sound);

    PlayMusicStream(music);

    game_logic.Init();
  }

  void Deinit() {
    omniman.TearDown();
    img.TearDown();
    UnloadImage(icon);
    UnloadSound(sound);  // Unload sound
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    game_logic.DeInit();
  }

  void Loop(void) {
    game_logic.Update();

    UpdateMusicStream(music);
    BeginDrawing();
    {
      game_logic.inputs.SetPlayerInputs();

      if (game_logic.inputs.playerInputs &
          static_cast<std::uint8_t>(Input::kJump)) {
        game_logic.player.Jump(game_logic.world_);
      }
      if (game_logic.inputs.playerInputs &
          static_cast<std::uint8_t>(Input::kLeft)) {
        game_logic.player.Move(false, game_logic.world_);
      } else if (game_logic.inputs.playerInputs &
                 static_cast<std::uint8_t>(Input::kRight)) {
        game_logic.player.Move(true, game_logic.world_);
      } else {
        game_logic.player.Decelerate(game_logic.world_);
      }

      ClearBackground(BLACK);

      for (auto collider : game_logic.Colliders) {
        collider.RenderColliderObject(game_logic.world_);
      }

      customScale += 0.01f;
      img.Draw(Vector2{GameLogic::screenWidth * 0.8f,
                       GameLogic::screenHeight * 0.5f});
      // omniman.Draw(center, customScale);

      DrawText("Coch-marre sample !!!", GameLogic::screenWidth / 2 - 200, 100,
               30, WHITE);
    }
    EndDrawing();
  }
};

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Loop();
}

int main() {
  InitWindow(GameLogic::screenWidth, GameLogic::screenHeight, "Online Game");

  Renderer renderer;
  renderer.Init();

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    renderer.Loop();
  }

#endif

  renderer.Deinit();

  return EXIT_SUCCESS;
}
