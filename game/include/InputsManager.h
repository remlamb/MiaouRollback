#pragma once
#include <cstdint>

namespace Input {
static constexpr std::uint8_t kJump = 1 << 0;
static constexpr std::uint8_t kRight = 1 << 1;
static constexpr std::uint8_t kLeft = 1 << 2;
static constexpr std::uint8_t kAttack = 1 << 3;
}  // namespace Input

class InputsManager {
 public:
  std::uint8_t playerInputs = 0;
  std::uint32_t frame = 0;
  void SetPlayerInputs();
};
