#include "GameApp.h"

#include "imgui_impl_raylib.h"

void GameApp::Init() {
  raylib::InitWindow(game::screen_width,
		game::screen_height, "Online Game");

	raylib::InitAudioDevice();
	game_renderer.Init();
	audio_manager.Init();
	InitImgui();
	game_logic.Init();
	game_logic.RegisterNetworkLogic(&networkLogic_);
	rollback_manager.RegisterGameManager(&game_logic);
	raylib::SetExitKey(KEY_NULL);
	Input::FrameInput::registerType();
}

void GameApp::InitImgui() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplRaylib_Init();
	Imgui_ImplRaylib_BuildFontAtlas();
	ImGui::StyleColorsClassic();

	ImGui::SetNextWindowSize(ImVec2(280, 400));
	ImGui::SetNextWindowPos(ImVec2(200, 100));

}

void GameApp::DrawImgui() {
	if (game_logic.current_game_state == game::GameState::LogMenu) {
		ImGui_ImplRaylib_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Main Menu");
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
			ImGui::Spacing();
			ImGui::Checkbox("Show Collider Shape", &isColliderVisible_);
			ImGui::Spacing();
			ImGui::SliderFloat("Volume", &audio_manager.audioVolume, 0.0f, 10.0f);
			ImGui::Spacing();
			ImGui::Checkbox("Play Sound", &audio_manager.isAudioPlaying);
			ImGui::Spacing();
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
	}


	if (game_logic.current_game_state == game::GameState::GameLaunch) {
		if (isGameOptionVisible)
		{
			ImGui_ImplRaylib_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Game Option");
			{
				ImGui::SliderFloat("Volume", &audio_manager.audioVolume, 0.0f, 10.0f);
				ImGui::Spacing();
				ImGui::Checkbox("Play Sound", &audio_manager.isAudioPlaying);
				ImGui::Spacing();
				ImGui::Checkbox("Show Collider Shape", &isColliderVisible_);
			}
			ImGui::End();
			ImGui::Render();
			ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
		}
	}

	if (game_logic.current_game_state == game::GameState::GameVictory)
	{
		ImGui_ImplRaylib_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("End Menu");
		{
			if (ImGui::Button("Disconnect", ImVec2(125, 25))) {
				networkLogic_.Disconnect();

			}
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
	}
}

void GameApp::Deinit() {
	game_renderer.Deinit();
	audio_manager.Deinit();
	raylib::CloseAudioDevice();
	raylib::CloseWindow();

	game_logic.DeInit();
	Input::FrameInput::unregisterType();
}

void GameApp::Loop(void) {
	game_logic.Update();
	networkLogic_.Run();

	if (game_logic.current_game_state == game::GameState::GameLaunch) {
		if (IsKeyReleased(KEY_ESCAPE)) {
			isGameOptionVisible = !isGameOptionVisible;
		}
	}

	ImGui_ImplRaylib_ProcessEvents();

	audio_manager.Update();
	raylib::BeginDrawing();
	{
		raylib::ClearBackground(raylib::BLACK);
		game_renderer.Draw(isColliderVisible_);
	}
	DrawImgui();
	raylib::EndDrawing();
}