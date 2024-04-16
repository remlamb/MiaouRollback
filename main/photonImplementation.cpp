#include <imgui.h>

#include "imgui_impl_raylib.h"
#include "PhotonApp.h"
//
//class App {
//public:
//    float packetDelayMin = 0.1f;
//    float packetDelayMax = 0.3f;
//    float packetLoose = 0.1f;
//
//    void Init() {
//        SetUpImGui();
//    }
//
//    void Deinit() {
//        CloseWindow();
//
//        ImGui_ImplRaylib_Shutdown();
//        ImGui::DestroyContext();
//    }
//
//    void Loop(void) {
//        ImGui_ImplRaylib_ProcessEvents();
//    }
//
//    void SetUpImGui() {
//        ImGui::CreateContext();
//        ImGuiIO& io = ImGui::GetIO();
//        (void)io;
//        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
//        ImGui_ImplRaylib_Init();
//        Imgui_ImplRaylib_BuildFontAtlas();
//        ImGui::StyleColorsClassic();
//    }
//
//    void CreateImGuiFrame() {
//        ImGui_ImplRaylib_NewFrame();
//        ImGui::NewFrame();
//
//        ImGui::Begin("Input network test.");
//        {
//            ImGui::SliderFloat("Delay Min", &packetDelayMin, 0.01f, 1.f);
//            ImGui::SliderFloat("Delay Max", &packetDelayMax, 0.01f, 1.f);
//            ImGui::SliderFloat("%PacketLoss", &packetLoose, 0.f, 1.f);
//        }
//        ImGui::End();
//    }
//
//    void DrawImGui() {
//        CreateImGuiFrame();
//        ImGui::Render();
//        ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
//    }
//};
//
//// Update and Draw one frame
//void UpdateDrawFrame(void* renderer) {
//    static_cast<App*>(renderer)->Loop();
//}

int main() {
//    InitWindow(1080, 720, "Photon App");
//    PhotonApp app{};
//    App renderer;
//    renderer.Init();
//
//#ifdef PLATFORM_WEB
//    emscripten_set_main_loop_arg(UpdateDrawFrame, &renderer, 0, 1);
//
//#else
//    while (!WindowShouldClose()) {
//        renderer.Loop();
//    }
//
//#endif
//
//    renderer.Deinit();

    return EXIT_SUCCESS;
}