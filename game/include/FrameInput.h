#pragma once
#include <Common-cpp/inc/CustomType.h>

#include <cstdint>

#include "Vec2.h"

namespace Input {
/**
 * @brief Represents input flags for various actions in the game.
 */
constexpr std::uint8_t kJump = 1 << 0;    // Flag for jump action.
constexpr std::uint8_t kRight = 1 << 1;   // Flag for moving right.
constexpr std::uint8_t kLeft = 1 << 2;    // Flag for moving left.
constexpr std::uint8_t kAttack = 1 << 3;  // Flag for attacking.

/**
 * @brief Represents player input for a single frame.
 *
 * The FrameInput class inherits from CustomType and encapsulates input data
 * for a specific frame, including the frame number and input flags.
 *
 * Variables:
 * - frame_nbr: The frame number associated with the input.
 * - input: Flags representing player input actions for the frame.
 */
class FrameInput final : public ExitGames::Common::CustomType<FrameInput, 1> {
  typedef CustomType<FrameInput, 1> super;

 public:
  FrameInput() noexcept = default;  // Default constructor.
  FrameInput(Math::Vec2F dir_to_mouse, short frame_nbr,
             std::uint8_t input) noexcept;  // Constructor with parameters.
  FrameInput(FrameInput&& toMove) noexcept = default;  // Move constructor.
  FrameInput& operator=(FrameInput&& toMove) noexcept =
      default;  // Move assignment operator.
  FrameInput(const FrameInput& toCopy) noexcept = default;  // Copy constructor.
  FrameInput& operator=(const FrameInput& toCopy) noexcept =
      default;                       // Copy assignment operator.
  ~FrameInput() override = default;  // Destructor.

  // Methods for CustomType implementation
  ExitGames::Common::JString& toString(
      ExitGames::Common::JString& retStr,
      bool withTypes) const override;  // Converts object to string.
  bool compare(const CustomTypeBase& other)
      const override;  // Compares with another object.
  void deserialize(const nByte* pData,
                   short length) override;  // Deserializes object from data.
  short serialize(nByte* pRetVal) const override;  // Serializes object to data.
  void duplicate(CustomTypeBase* pRetVal) const override;  // Duplicates object.

  bool operator==(
      const FrameInput& other) const noexcept;  // Equality comparison operator.

  void UpdatePlayerInputs();  // Updates player inputs based on keyboard and gamepad states.

  short frame_nbr = 0;  // The frame number associated with the input.
  std::uint8_t input =
      0;  // Flags representing player input actions for the frame.

  static nByte
      serialization_protocol;  // Serialization protocol for FrameInput objects.
};
}  // namespace Input
