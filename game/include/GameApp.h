#pragma once
#include "GameLogic.h"
#include "NetworkLogic.h"
#include "Renderer.h"

class GameApp {
 public:
  game::GameLogic* game_logic;
  Renderer* game_renderer;

  raylib::Sound sound;
  raylib::Music music;

  GameApp(game::GameLogic* gl, Renderer* renderer)
      : game_logic(gl), game_renderer(renderer) {}

  void Init();
  void InitImgui();
  void DrawImgui();
  void Deinit();
  void Loop(void);

  ExitGames::Common::JString appID =
      L"d7a7fb07-5b89-4563-af7f-a83778fe14f8";  // set your app id here
  ExitGames::Common::JString appVersion = L"1.0";
  NetworkLogic networkLogic_{appID, appVersion, game_logic};
};
