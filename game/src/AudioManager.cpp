#include "AudioManager.h"

void AudioManager::Init() noexcept {
  music = raylib::LoadMusicStream("data/theme.wav");
  music.looping = true;
}

void AudioManager::Deinit() noexcept {
  UnloadMusicStream(music);
  raylib::CloseAudioDevice();
}

void AudioManager::Update() const noexcept {
  if (game_logic_->current_game_state != game::GameState::GameLaunch) {
    return;
  }
  if (is_audio_playing) {
    PlayMusicStream(music);
    SetMusicVolume(music, audio_volume);
    UpdateMusicStream(music);
  }
}