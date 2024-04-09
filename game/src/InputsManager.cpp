#include "InputsManager.h"

#include <raylib.h>

void InputsManager::SetPlayerInputs() {
  playerInputs = 0;
  if (IsKeyDown(KEY_SPACE) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
    playerInputs |= static_cast<std::uint8_t>(Input::kJump);
  }
  if (IsKeyDown(KEY_D) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
    playerInputs |= static_cast<std::uint8_t>(Input::kRight);
  }
  if (IsKeyDown(KEY_A) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
    playerInputs |= static_cast<std::uint8_t>(Input::kLeft);
  }
}
