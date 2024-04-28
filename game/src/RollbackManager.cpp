#include "RollbackManager.h"

void RollbackManager::SetLocalPlayerInput(const Input::FrameInput& local_input,
    int player_id) noexcept {
    inputs_[player_id][local_input.frame_nbr] = local_input;
    last_inputs_[player_id] = local_input;
}

void RollbackManager::SetRemotePlayerInput(
    const std::vector<Input::FrameInput>& new_remote_inputs, int player_id) {
    // Retrieve the last remote frame input
    auto last_new_remote_input = new_remote_inputs.back();

    // Calculate the difference between the last new remote frame and the last remote
    // input frame
    const auto frame_diff = last_new_remote_input.frame_nbr - last_remote_input_frame_;

    // If no new inputs received, return
    if (frame_diff < 1) {
        return;
    }

    // If the last remote input frame is greater than the current frame, adjust
    // last_new_remote_input
    if (last_new_remote_input.frame_nbr > current_frame_) {
        const auto& current_frame_it =
            std::find_if(new_remote_inputs.begin(), new_remote_inputs.end(),
                [this](const Input::FrameInput& frame_input) {
                    return frame_input.frame_nbr == current_frame_;
                });
        last_new_remote_input = *current_frame_it;
    }

    // Find the position of the first missing input
    auto missing_input_it = std::find_if(
        new_remote_inputs.begin(), new_remote_inputs.end(),
        [this](const Input::FrameInput& frame_input) {
            return frame_input.frame_nbr == last_remote_input_frame_ + 1;
        });

    // Check if rollback is necessary
    bool must_rollback = last_remote_input_frame_ == -1;

    // Iterate over the missing inputs and update the inputs array
    for (short frame = last_remote_input_frame_ + 1;
        frame <= last_new_remote_input.frame_nbr; frame++) {
        // Get the input for the current frame
        const auto input = missing_input_it->input;

        // Check if rollback is necessary
        if (last_remote_input_frame_ > -1 && input != last_inputs_[player_id].input) {
            must_rollback = true;
        }

        // Update the inputs array
        inputs_[player_id][frame] = *missing_input_it;

        // Move to the next missing input
        ++missing_input_it;
    }

    // Predict inputs for frames up to the current frame with the last remote input.
    for (short frame = last_new_remote_input.frame_nbr;
        frame <= current_frame_; frame++) {
        inputs_[player_id][frame] = last_new_remote_input;
    }

    // Rollback if necessary.
    if (must_rollback) {
        SimulateUntilCurrentFrame();
    }

    // Update last inputs and last remote input frame.
    last_inputs_[player_id] = last_new_remote_input;
    last_remote_input_frame_ = last_new_remote_input.frame_nbr;
}


void RollbackManager::SimulateUntilCurrentFrame() const noexcept {
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif

    current_game_manager_->Rollback(confirmed_game_manager_);

    for (short frame = static_cast<short>(confirmed_frame_ + 1);
        frame < current_frame_; frame++) {
        //Todo maxPlayer au lieu de 2 
        for (int player_id = 0; player_id < 2;
            player_id++) {
            const auto input = inputs_[player_id][frame];
            current_game_manager_->SetPlayerInput(input, player_id);
        }
        current_game_manager_->UpdateGameplay();
    }
    // The Fixed update of the current frame is made in the main loop after polling
    // received events from network.
}

int RollbackManager::ConfirmFrame() noexcept {
    //Todo maxPlayer au lieu de 2
    std::cout << "Confirm ?" << std::endl;
    for (int player_id = 0; player_id < 2;
        player_id++) {
        const auto input = inputs_[player_id][frame_to_confirm_];
        confirmed_game_manager_.SetPlayerInput(input, player_id);
    }

    confirmed_game_manager_.UpdateGameplay();
    const auto checksum = confirmed_game_manager_.ComputeChecksum();

    confirmed_frame_++;
    frame_to_confirm_++;

    return checksum;
}

const Input::FrameInput& RollbackManager::GetLastPlayerConfirmedInput(
    const int player_id) const noexcept {
    return last_inputs_[player_id];
}
