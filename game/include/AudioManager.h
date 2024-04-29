#pragma once
#include "GameLogic.h"

class AudioManager {
 public:
  AudioManager(game::GameLogic* game_logic) { game_logic_ = game_logic; }
  raylib::Sound sound;
  raylib::Music music;
  void Init();
  void Deinit();
  void Update();
private:
  game::GameLogic* game_logic_ = nullptr;
};