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
  // raylib::DrawRectangle(700, 500, 100, 100, raylib::Color{ 255, 0, 0, 255 });

  // raylib::DrawRaylibText("Raylib drawing text with photon in the same app.",
  //     400, 350, 30, raylib::kWhite);
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

    if (ImGui::Button("Disconnect", ImVec2(125, 25))) {
      networkLogic_.Disconnect();
    }

    ImGui::Spacing();

    if (ImGui::Button("Join or create Room", ImVec2(125, 25))) {
      networkLogic_.JoinRandomOrCreateRoom();
    }

    ImGui::Spacing();

    if (ImGui::Button("Join Room", ImVec2(125, 25))) {
      networkLogic_.JoinRandomRoom();
    }

    ImGui::Spacing();

    if (ImGui::Button("Send event", ImVec2(125, 25))) {
      ExitGames::Common::Hashtable evData;
      evData.put<nByte, int>(static_cast<nByte>(EventKey::kPlayerInput), 42);

      constexpr bool sendReliable = false;
      networkLogic_.RaiseEvent(sendReliable, EventCode::kInput, evData);
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