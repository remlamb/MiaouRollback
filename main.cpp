#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

static constexpr int screenWidth = 800;
static constexpr int screenHeight = 650;

class Renderer {
 public:
  Texture2D penguin;
  Vector2 position;
  Sound music;

  void Init() {
    penguin = LoadTexture("data/2222.png");
    position = {screenWidth * 0.5f - penguin.width,
                screenHeight * 0.5f - penguin.height};

    InitAudioDevice();

    music = LoadSound("data/music.wav");

    PlaySound(music);
  }

  void Deinit() {
    UnloadTexture(penguin);
    UnloadSound(music);  // Unload sound

    CloseAudioDevice();
    CloseWindow();
  }

  void Loop(void) {
    BeginDrawing();
    {
      ClearBackground(BLACK);

      DrawTextureEx(penguin, position, 0.f, 4.f, WHITE);

      DrawText("Raylib sample !!!", 290, 240, 30, WHITE);
    }
    EndDrawing();
  }
};

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
  static_cast<Renderer*>(renderer)->Loop();
}

int main() {
  InitWindow(screenWidth, screenHeight, "Raylib sample");

  Renderer renderer;
  renderer.Init();

#ifdef PLATFORM_WEB
  emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // UpdateDrawFrame();
    renderer.Loop();
  }

#endif

  renderer.Deinit();

  return EXIT_SUCCESS;
}
