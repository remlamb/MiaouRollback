#pragma once
#include "AudioManager.h"
#include "GameLogic.h"
#include "NetworkLogic.h"
#include "Renderer.h"
#include "RollbackManager.h"

/**
 * @brief Represents the main game application.
 *
 * The GameApp class manages the main game loop, initialization, and
 * deinitialization of various game components including game logic, rendering,
 * audio, and networking. It also handles user interface interactions using Dear
 * ImGui for displaying menus and game options.
 *
 * Variables:
 * - is_collider_visible_: Indicates whether collider shapes are visible.
 * - is_game_option_visible_ : Indicates whether the game options menu is
 * visible.
 * - appID: The application ID for networking purposes.
 * - appVersion: The application version for networking purposes.
 * - rollback_manager: Manages game state rollback for network synchronization.
 * - game_logic: Manages the game logic and state.
 * - game_renderer: Handles rendering of the game world.
 * - audio_manager: Manages audio playback and volume settings.
 * - networkLogic_: Handles networking logic and communication.
 */
class GameApp {
 private:
  bool is_collider_visible_ =
      false;  // Indicates whether collider shapes are visible.
  bool is_game_option_visible_ =
      false;  // Indicates whether the game options menu is visible.

 public:
  RollbackManager rollback_manager;  // Manages game state rollback for network synchronization.
  game::GameLogic game_logic{
      &rollback_manager};  // Manages the game logic and state.
  Renderer game_renderer{
      &game_logic, &network_logic};  // Handles rendering of the game world.
  AudioManager audio_manager{
      &game_logic};  // Manages audio playback and volume settings.

  ExitGames::Common::JString appID =
      L"d7a7fb07-5b89-4563-af7f-a83778fe14f8";  // The Photon application ID for networking purposes.
  ExitGames::Common::JString appVersion =
      L"1.0";  // The Photon application version.
  NetworkLogic network_logic{
      appID, appVersion,
      &game_logic};  // Handles networking logic and communication.

  /**
   * @brief Initializes the game application, create Window, init logic, manager
   * and renderer.
   */
  void Init();
  /**
   * @brief Initializes Dear ImGui for the game interface.
   */
  void InitImgui();
  /**
   * @brief Draws the Dear ImGui interface according to the current game state.
   */
  void DrawImgui();
  /**
   * @brief Deinitializes the game application.
   */
  void Deinit();
  /**
   * @brief Runs the main game loop.
   */
  void Loop(void);
};
