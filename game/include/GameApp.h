#pragma once
#include "GameLogic.h"
#include "NetworkLogic.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "RollbackManager.h"

class GameApp {
public:
	RollbackManager rollback_manager;
	game::GameLogic game_logic{ &rollback_manager};
	Renderer game_renderer{ &game_logic };
	AudioManager audio_manager;

	void Init();
	void InitImgui();
	void DrawImgui();
	void Deinit();
	void Loop(void);

	ExitGames::Common::JString appID =
		L"d7a7fb07-5b89-4563-af7f-a83778fe14f8";  // set your app id here
	ExitGames::Common::JString appVersion = L"1.0";
	NetworkLogic networkLogic_{ appID, appVersion, &game_logic };
};