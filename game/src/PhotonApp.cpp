#include "PhotonApp.h"

#include <imgui.h>

#include "imgui_impl_raylib.h"
#include "raylib_wrapper.h"

void PhotonApp::Setup() noexcept {
  raylib::InitWindow(1080, 720, "Photon App");

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplRaylib_Init();
  Imgui_ImplRaylib_BuildFontAtlas();
  ImGui::StyleColorsClassic();
}

void PhotonApp::Update() noexcept {
  networkLogic_.Run();
  ImGui_ImplRaylib_ProcessEvents();
}

void PhotonApp::Draw() noexcept {
}

void PhotonApp::DrawImGui() noexcept {
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
}

void PhotonApp::TearDown() noexcept {
  networkLogic_.Disconnect();
  raylib::CloseWindow();

  ImGui_ImplRaylib_Shutdown();
  ImGui::DestroyContext();
}