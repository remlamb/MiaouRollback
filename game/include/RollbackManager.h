#pragma once

#include "FrameInput.h"
#include "GameLogic.h"

/**
 * @brief RollbackManager is a class responsible for maintaining the integrity of the game simulation.
 *
 * It manages three game states with different temporalities:
 * - The current game state represents the local client's real-time game.
 * - The confirmed state is the last game state confirmed by the client (verified through checksum).
 * - The state to be confirmed is the game state calculated by the master client once all inputs for a frame
 *   have been received. Other clients receive the checksum of this game state and verify its integrity against
 *   their own checksum for the same state.
 */
class RollbackManager {
public:
	/**
   * @brief Registers the GameManager for the RollbackManager.
   *
   * @param current_game_manager Pointer to the local client's GameManager.
   */
	void RegisterGameManager(game::GameLogic* current_game_manager) noexcept {
		current_game_manager_ = current_game_manager;
		confirmed_game_manager_.Init();

		for (std::size_t i = 0; i < game::max_player; i++) {
			inputs_[i].resize(kMaxFrameCount);
		}
	}

	/**
   * @brief Sets the local player's input for a specific frame.
   *
   * @param local_input The input for the local player.
   * @param player_id The ID of the local player.
   */
	void SetLocalPlayerInput(const Input::FrameInput& local_input,
		int player_id) noexcept;

	/**
   * @brief Sets the remote player's input for a specific frame.
   *
   * @param new_remote_inputs The inputs for the remote player.
   * @param player_id The ID of the remote player.
   */
	void SetRemotePlayerInput(
		const std::vector<Input::FrameInput>& new_remote_inputs, int player_id);

	/**
	 * @brief Simulates the game until the current frame.
	 */
	void SimulateUntilCurrentFrame() const noexcept;

	/**
	 * @brief Confirms the current frame and advances to the next frame.
	 *
	 * @return The frame number that has been confirmed.
	 */
	int ConfirmFrame() noexcept;


	/**
	 * @brief Retrieves the last confirmed input for a specific player.
	 *
	 * @param player_id The ID of the player.
	 * @return The last confirmed input for the player.
	 */
	[[nodiscard]] const Input::FrameInput& GetLastPlayerConfirmedInput(
		int player_id) const noexcept;


	/**
	 * @brief Retrieves the current frame number.
	 *
	 * @return The current frame number.
	 */
	[[nodiscard]] short current_frame() const noexcept { return current_frame_; }

	/**
	 * @brief Increments the current frame number.
	 */
	void IncreaseCurrentFrame() noexcept { current_frame_++; }


	/**
	 * @brief Retrieves the last confirmed frame number.
	 *
	 * @return The last confirmed frame number.
	 */
	[[nodiscard]] short confirmed_frame() const noexcept {
		return confirmed_frame_;
	}

	/**
	 * @brief Retrieves the frame number of the last received remote input.
	 *
	 * @return The frame number of the last received remote input.
	 */
	[[nodiscard]] short last_remote_input_frame() const noexcept {
		return last_remote_input_frame_;
	}

	/**
	 * @brief Retrieves the frame number to be confirmed by the master client.
	 *
	 * @return The frame number to be confirmed.
	 */
	[[nodiscard]] short frame_to_confirm() const noexcept {
		return frame_to_confirm_;
	}

	/**
	 * @brief Resets the RollbackManager to its initial state.
	 */
	void Reset() noexcept {
		current_frame_ = -1;
		last_remote_input_frame_ = -1;
		frame_to_confirm_ = 0;
		confirmed_frame_ = -1;
		for (auto& input : inputs_) {
			input.clear();
		}
		last_inputs_.fill({});
	}


	game::GameLogic confirmed_game_manager_{ this }; 	/* confirmed_player_manager_ is a copy of the client's player_manager at the last confirmed frame state. */

private:

	game::GameLogic* current_game_manager_ = nullptr; 	/* current_game_manager_ is a pointer to local client's GameManager.
	 */


	short current_frame_ = -1; 	/* The frame nbr of the local client.
	 */


	short last_remote_input_frame_ = -1; 	/* The frame number of the last time a remote input was received.
	 */


	short frame_to_confirm_ = 0; 	/* The frame number which the master client wants to confirm.
	 */


	short confirmed_frame_ = -1; 	/* The frame number of the last confirmed frame (frame verified with checksum).
	 */


	static constexpr short kMaxFrameCount = 30'000; 	/* kMaxFrameCount is the maximum of frame that the game can last. Here 30'000 corresponds to 10 minutes at a fixed 50fps.
	 */

	std::array<std::vector<Input::FrameInput>, game::max_player> inputs_{};

	std::array<Input::FrameInput, game::max_player> last_inputs_{}; 	/* last_inputs_ is an array which stores the last inputs received by the different players.
	 */
};
