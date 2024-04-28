#include "AudioManager.h"


void AudioManager::Init() {
	sound = raylib::LoadSound("data/walkman.wav");
	music = raylib::LoadMusicStream("data/theme.wav");
	music.looping = true;

	//PlayMusicStream(music);
}


void AudioManager::Deinit() {
	UnloadSound(sound);  // Unload sound
	UnloadMusicStream(music);
	raylib::CloseAudioDevice();
}


void AudioManager::Update() {
	//UpdateMusicStream(music);
}