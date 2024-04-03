#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

#include "Image.h"

static constexpr int screenWidth = 800;
static constexpr int screenHeight = 650;

class Renderer {
 public:
  Sound music;

  ImageCustom omniman;
  float customScale = 0.0f;
  Vector2 center = {screenWidth * 0.5f, screenHeight * 0.5f};

  void Init() {
    omniman.Setup("data/omniman.png", 1.8f, Pivot::Center);
    InitAudioDevice();
    music = LoadSound("data/music.wav");
    PlaySound(music);
  }

  void Deinit() {
    omniman.TearDown();
    UnloadSound(music);  // Unload sound

    CloseAudioDevice();
    CloseWindow();
  }

  void Loop(void) {
    BeginDrawing();
    {
      ClearBackground(BLACK);
      customScale += 0.01f;
      omniman.Draw(center, customScale);

      DrawText("Raylib sample !!!", 290, 100, 30, WHITE);
    }
    EndDrawing();
  }
};

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Loop();
}

ImageCustom img;

int main() {
  InitWindow(screenWidth, screenHeight, "WakfuRaylibsssss");

  Renderer renderer;
  renderer.Init();
  img.Setup("data/2222.png", 1.0f, Pivot::Center);

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // UpdateDrawFrame();
    renderer.Loop();
    img.Draw(Vector2{screenWidth * 0.8f, screenHeight * 0.5f});
  }

#endif

  renderer.Deinit();
  img.TearDown();

  return EXIT_SUCCESS;
}
