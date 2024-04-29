#include "AudioManager.h"

void AudioManager::Init() {
  sound = raylib::LoadSound("data/walkman.wav");
  music = raylib::LoadMusicStream("data/theme.wav");
  music.looping = true;
}

void AudioManager::Deinit() {
  UnloadSound(sound);  // Unload sound
  UnloadMusicStream(music);
  raylib::CloseAudioDevice();
}

void AudioManager::Update() {
  if (game_logic_->current_game_state != game::GameState::GameLaunch) {
	 return;
  }
  //PlayMusicStream(music);
  //UpdateMusicStream(music);
}