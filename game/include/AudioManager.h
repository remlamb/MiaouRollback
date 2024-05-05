#pragma once
#include "GameLogic.h"

/**
 * @brief Represents the AudioManager responsible for managing audio in the
 * game.
 *
 * The AudioManager class handles loading, playing, and updating background
 * music for the game. It provides methods for initialization, deinitialization,
 * and updating the audio based on the current game state and volume settings.
 *
 * * Variables:
 * - is_audio_playing: Indicates whether audio is currently playing.
 * - audio_volume: The volume level of the audio.
 * - music: The music stream object.
 * - game_logic_: Pointer to the GameLogic object associated with the AudioManager.
 */
class AudioManager {
 public:
  bool is_audio_playing =
      true;                   // Indicates whether audio is currently playing.
  float audio_volume = 0.5f;  // The volume level of the audio.

  /**
   * @brief Constructs an AudioManager object with a pointer to the game logic.
   * @param game_logic Pointer to the GameLogic object.
   */
  AudioManager(game::GameLogic* game_logic) { game_logic_ = game_logic; }
  raylib::Music music;  // The music stream object.
  /**
   * @brief Initializes the AudioManager.
   * Loads the background music from file and sets looping.
   */
  void Init() noexcept;
  /**
   * @brief Deinitializes the AudioManager.
   * Unloads the background music and closes the audio device.
   */
  void Deinit() noexcept;
  /**
   * @brief Updates the AudioManager.
   * Plays or stops the music based on the current game state and volume
   * settings.
   */
  void Update() const noexcept;

 private:
  game::GameLogic* game_logic_ = nullptr;  // Pointer to the GameLogic object.
};