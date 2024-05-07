#include "GameApp.h"

#include "imgui_impl_raylib.h"

void GameApp::Init() {
  raylib::InitWindow(game::screen_width, game::screen_height, game::game_name);

  raylib::InitAudioDevice();
  game_renderer.Init();
  audio_manager.Init();
  InitImgui();
  game_logic.Init();
  game_logic.RegisterNetworkLogic(&network_logic);
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
}

void GameApp::DrawImgui() {
  ImGui_ImplRaylib_NewFrame();
  ImGui::NewFrame();
  if (game_logic.current_game_state == game::GameState::LogMenu) {
    ImGui::SetNextWindowSize(ImVec2(280, 600));
    ImGui::SetNextWindowPos(ImVec2(80, 80));

    ImGui::Begin("Main Menu");
    {
      ImGui::Text("--- Online: ---");
      ImGui::Spacing();
      if (!network_logic.is_connected) {
        if (ImGui::Button("Connect", ImVec2(125, 25))) {
          network_logic.Connect();
        }
        ImGui::Spacing();
      }

      if (network_logic.is_connected) {
        if (ImGui::Button("Join Game", ImVec2(125, 25))) {
          network_logic.JoinRandomOrCreateRoom();
        }

        ImGui::Spacing();

        if (ImGui::Button("Disconnect", ImVec2(125, 25))) {
          network_logic.Disconnect();
        }
      }
      ImGui::Spacing();
      ImGui::Text("");
      ImGui::Text("--- Option: ---");
      ImGui::Spacing();
      ImGui::SliderFloat("Volume", &audio_manager.audio_volume, 0.0f, 10.0f);
      ImGui::Spacing();
      ImGui::Checkbox("Play Sound", &audio_manager.is_audio_playing);
      ImGui::Spacing();
      ImGui::Checkbox("Show Collider Shape", &is_collider_visible_);
      ImGui::Spacing();

      ImGui::Text("");
      ImGui::Spacing();
      ImGui::Text("--- Control: ---");
      ImGui::Spacing();
      ImGui::TextWrapped("Use 'A' and 'D' to Move on the X axis");
      ImGui::Spacing();
      ImGui::TextWrapped("Use 'Space' or 'W' to Jump");
      ImGui::Spacing();
      ImGui::TextWrapped("Use 'S' or 'Left Alt' to Shoot");
      ImGui::Spacing();

      ImGui::Text("");
      ImGui::Spacing();
      ImGui::Text("--- Control with Controller: ---");
      ImGui::Spacing();
      ImGui::TextWrapped("Use Left and Right* to Move");
      ImGui::Spacing();
      ImGui::TextWrapped("Use Up* or South Button** to Jump");
      ImGui::Spacing();
      ImGui::TextWrapped("Use Down* or West Button** to Shoot");
      ImGui::Spacing();
      ImGui::Text("");
      ImGui::TextWrapped("*D-pad Arrow");
      ImGui::Spacing();
      ImGui::TextWrapped("**Xbox Controller: South Btn = A,  West Btn = X");
      ImGui::Spacing();
      ImGui::TextWrapped(
          "**Sony Controller: South Btn = X,  West Btn = Square");
    }
  }

  if (game_logic.current_game_state == game::GameState::GameLaunch) {
    if (is_game_option_visible_) {
      ImGui::Begin("Game Option");
      {
        ImGui::SliderFloat("Volume", &audio_manager.audio_volume, 0.0f, 10.0f);
        ImGui::Spacing();
        ImGui::Checkbox("Play Sound", &audio_manager.is_audio_playing);
        ImGui::Spacing();
        ImGui::Checkbox("Show Collider Shape", &is_collider_visible_);
      }
    }
  }

  if (game_logic.current_game_state == game::GameState::GameVictory) {
    ImGui::SetNextWindowSize(ImVec2(280, 600));
    ImGui::SetNextWindowPos(ImVec2(80, 80));
    ImGui::Begin("Ending Menu");
    {
      ImGui::TextWrapped("--- Game: ---");
      ImGui::Spacing();
      if (ImGui::Button("Return to Main Menu", ImVec2(125, 25))) {
        rollback_manager.Reset();
      	game_logic.ResetState();
        rollback_manager.RegisterGameManager(&game_logic);
        network_logic.Disconnect();
      }
      ImGui::Spacing();
      if (ImGui::Button("Quit", ImVec2(125, 25))) {
        network_logic.Disconnect();
        Deinit();
      }
      ImGui::Spacing();
      ImGui::Text("");
      ImGui::TextWrapped("--- More: ---");
      ImGui::Spacing();
      ImGui::TextWrapped("Thank you for playing");
      ImGui::Spacing();
    }
  }
  ImGui::End();
  ImGui::Render();
  ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
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
  network_logic.Run();

  if (game_logic.current_game_state == game::GameState::GameLaunch) {
    if (IsKeyReleased(KEY_ESCAPE)) {
      is_game_option_visible_ = !is_game_option_visible_;
    }
  }

  ImGui_ImplRaylib_ProcessEvents();

  audio_manager.Update();
  raylib::BeginDrawing();
  {
    raylib::ClearBackground(raylib::BLACK);
    game_renderer.Draw(is_collider_visible_);
  }
  DrawImgui();
  raylib::EndDrawing();
}