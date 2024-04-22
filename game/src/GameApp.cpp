#include "GameApp.h"

#include "imgui_impl_raylib.h"

void GameApp::Init() {
	raylib::InitWindow(game::GameLogic::screenWidth,
		game::GameLogic::screenHeight, "Online Game");

	raylib::InitAudioDevice();
	game_renderer->Init();
	sound = raylib::LoadSound("data/walkman.wav");
	music = raylib::LoadMusicStream("data/music.wav");
	music.looping = true;

	PlayMusicStream(music);
	InitImgui();
	game_logic->Init();
}

void GameApp::InitImgui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplRaylib_Init();
	Imgui_ImplRaylib_BuildFontAtlas();
	ImGui::StyleColorsClassic();
}

void GameApp::DrawImgui()
{
	if (game_logic->current_game_state == game::GameState::LogMenu)
	{
		ImGui_ImplRaylib_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(150, 210));
		ImGui::SetNextWindowPos(ImVec2(200, 100));
		ImGui::Begin("Network UI");
		{
			if (ImGui::Button("Connect", ImVec2(125, 25))) {
				networkLogic_.Connect();
			}

			ImGui::Spacing();

			if (networkLogic_.is_connected) {
				if (ImGui::Button("Join Game", ImVec2(125, 25))) {
					networkLogic_.JoinRandomOrCreateRoom();
				}

				ImGui::Spacing();

				if (ImGui::Button("Disconnect", ImVec2(125, 25))) {
					networkLogic_.Disconnect();
				}
			}
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
	}
}

void GameApp::Deinit() {
	game_renderer->Deinit();
	UnloadSound(sound);  // Unload sound
	UnloadMusicStream(music);
	raylib::CloseAudioDevice();
	raylib::CloseWindow();

	game_logic->DeInit();
}

void GameApp::Loop(void) {
	game_logic->Update();
	networkLogic_.Run();

	ImGui_ImplRaylib_ProcessEvents();

	UpdateMusicStream(music);
	raylib::BeginDrawing();
	{
		raylib::ClearBackground(raylib::BLACK);
		game_renderer->Draw();
	}
	DrawImgui();
	raylib::EndDrawing();
}