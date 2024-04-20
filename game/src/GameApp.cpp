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

  game_logic->Init();

  // Photon
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplRaylib_Init();
  Imgui_ImplRaylib_BuildFontAtlas();
  ImGui::StyleColorsClassic();
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

  ImGui_ImplRaylib_NewFrame();
  ImGui::NewFrame();

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

  raylib::EndDrawing();
}