#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include <iostream>

#include "Image.h"
#include "GameLogic.h"

static constexpr int screenWidth = 1080;
static constexpr int screenHeight = 720;

class Renderer {
public:
	GameLogic game_logic;
	Sound sound;
	Music music;

	ImageCustom img;
	ImageCustom omniman;
	Image icon;
	float customScale = 0.0f;
	Vector2 center = { screenWidth * 0.5f, screenHeight * 0.5f };

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
			if (IsKeyDown(KEY_SPACE) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
				game_logic.Jump();
			}

			ClearBackground(BLACK);

			game_logic.RenderColliderObject();

			customScale += 0.01f;
			img.Draw(Vector2{ screenWidth * 0.8f, screenHeight * 0.5f });
			//omniman.Draw(center, customScale);

			DrawText("Coch-marre sample !!!", screenWidth / 2 - 200, 100, 30, WHITE);
		}
		EndDrawing();
	}
};

// Update and Draw one frame
void UpdateDrawFrame(void* renderer) {
	static_cast<Renderer*>(renderer)->Loop();
}

int main() {
	InitWindow(screenWidth, screenHeight, "Online Game");

	Renderer renderer;
	renderer.Init();

#ifdef PLATFORM_WEB
	emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);

#else
	SetTargetFPS(100);

	while (!WindowShouldClose()) {
		renderer.Loop();
	}

#endif

	renderer.Deinit();

	return EXIT_SUCCESS;
}
