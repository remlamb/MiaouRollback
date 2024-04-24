#include "FrameInput.h"

void Input::FrameInput::UpdatePlayerInputs() {
  playerInput = 0;
  if (IsKeyDown(KEY_SPACE) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
    playerInput |= static_cast<std::uint8_t>(Input::kJump);
  }
  if (IsKeyDown(KEY_D) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
    playerInput |= static_cast<std::uint8_t>(Input::kRight);
  }
  if (IsKeyDown(KEY_A) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
    playerInput |= static_cast<std::uint8_t>(Input::kLeft);
  }
}
