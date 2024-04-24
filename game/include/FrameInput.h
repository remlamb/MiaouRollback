#pragma once
#include <cstdint>

#include "raylib_wrapper.h"
using namespace raylib;

namespace Input {
constexpr std::uint8_t kJump = 1 << 0;
constexpr std::uint8_t kRight = 1 << 1;
constexpr std::uint8_t kLeft = 1 << 2;
constexpr std::uint8_t kAttack = 1 << 3;

struct FrameInput {
  std::uint8_t playerInput = 0;
  int frame = 0;

  void UpdatePlayerInputs();
};
}  // namespace Input
