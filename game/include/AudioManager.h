#pragma once
#include "raylib_wrapper.h"

class AudioManager
{
public:
	raylib::Sound sound;
	raylib::Music music;
	void Init();
	void Deinit();
	void Update();
};