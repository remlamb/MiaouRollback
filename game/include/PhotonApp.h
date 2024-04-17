#pragma once
#include "NetworkLogic.h"


class PhotonApp
{
public:
	void Setup() noexcept;
	void Update() noexcept;
	void Draw() noexcept;
	void DrawImGui() noexcept;
	void TearDown() noexcept;

	float packetDelayMin = 0.1f;
    float packetDelayMax = 0.3f;
    float packetLoose = 0.1f;

private:
	ExitGames::Common::JString appID =
		L"d7a7fb07-5b89-4563-af7f-a83778fe14f8";  // set your app id here
	ExitGames::Common::JString appVersion = L"1.0";
	NetworkLogic networkLogic_{ appID, appVersion };
};

