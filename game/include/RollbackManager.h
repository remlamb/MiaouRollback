#pragma once

#include "FrameInput.h"
#include "GameLogic.h"

/**
 * \brief RollbackManager is a class responsible of the integrity of the game
 * simulation.
 *
 * It has 3 game states with different temporalities.
 *
 * The current game state, which is the local client's real-time game.
 *
 * The confirmed state, which is the last game state confirmed by the client.
 * (A checksum is done to confirm the integrity of the simulation.)
 *
 * Finally, the state to be confirmed is the game state calculated by the
 * master client once all the inputs for a frame have been received.
 * All other clients receive the checksum of this game state and verify if it
 * corresponds to their checksum for this given state.
 */
class RollbackManager {
public:
	void RegisterGameManager(game::GameLogic* current_game_manager) noexcept {
		current_game_manager_ = current_game_manager;
		confirmed_game_manager_.Init();

		//todo change 2 to game_constants::kMaxPlayerCount
		for (std::size_t i = 0; i < 2; i++) {
			inputs_[i].resize(kMaxFrameCount);
		}
	}

	void SetLocalPlayerInput(const Input::FrameInput& local_input, int player_id) noexcept;
	void SetRemotePlayerInput(const std::vector<Input::FrameInput>& new_remote_inputs,
		int player_id);

	void SimulateUntilCurrentFrame() const noexcept;
	int ConfirmFrame() noexcept;

	[[nodiscard]] const Input::FrameInput& GetLastPlayerConfirmedInput(
		int player_id) const noexcept;

	[[nodiscard]] short current_frame() const noexcept {
		return current_frame_;
	}

	void IncreaseCurrentFrame() noexcept { current_frame_++; }

	[[nodiscard]] short confirmed_frame() const noexcept {
		return confirmed_frame_;
	}

	[[nodiscard]] short last_remote_input_frame() const noexcept {
		return last_remote_input_frame_;
	}

	[[nodiscard]] short frame_to_confirm() const noexcept {
		return frame_to_confirm_;
	}

	/**
 * \brief confirmed_player_manager_ is a copy of the client's player_manager
 * at the last confirmed frame state.
 */
	game::GameLogic confirmed_game_manager_{ this };

private:
	/**
	 * \brief current_game_manager_ is a pointer to local client's GameManager.
	 */
	game::GameLogic* current_game_manager_ = nullptr;



	/**
	 * \brief The frame nbr of the local client.
	 */
	short current_frame_ = -1;

	/**
	 * \brief The frame number of the last time a remote input was received.
	 */
	short last_remote_input_frame_ = -1;

	/**
	 * \brief The frame number which the master client wants to confirm.
	 */
	short frame_to_confirm_ = 0;

	/**
	 * \brief The frame number of the last confirmed frame (frame verified with
	 * checksum).
	 */
	short confirmed_frame_ = -1;

	/**
	 * \brief kMaxFrameCount is the maximum of frame that the game can last.
	 * Here 30'000 corresponds to 10 minutes at a fixed 50fps.
	 */
	static constexpr short kMaxFrameCount = 30'000;

	std::array<std::vector<Input::FrameInput>,
		2> inputs_{};
	/**
	 * \brief last_inputs_ is an array which stores the last inputs received by the
	 * different players.
	 */
	std::array<Input::FrameInput, 2> last_inputs_{};
};

