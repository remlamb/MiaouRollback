#include "FrameInput.h"
#include <Common-cpp/inc/Common.h>
#include "raylib_wrapper.h"
using namespace raylib;

void Input::FrameInput::UpdatePlayerInputs() {
	input = 0;
	if (IsKeyDown(KEY_SPACE) ||
            IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
            IsKeyDown(KEY_W)) {
		input |= static_cast<std::uint8_t>(Input::kJump);
	}
	if (IsKeyDown(KEY_D) ||
		IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
		input |= static_cast<std::uint8_t>(Input::kRight);
	}
	if (IsKeyDown(KEY_A) ||
		IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
		input |= static_cast<std::uint8_t>(Input::kLeft);
	}
	if (IsKeyDown(KEY_S) ||
		IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
		input |= static_cast<std::uint8_t>(Input::kAttack);
	}
}

namespace  Input
{
	nByte FrameInput::serialization_protocol =
		ExitGames::Common::SerializationProtocol::DEFAULT;

	FrameInput::FrameInput(Math::Vec2F dir_to_mouse, short frame_nbr,
		uint8_t input) noexcept
		: frame_nbr(frame_nbr), input(input) {}

	ExitGames::Common::JString& FrameInput::toString(
		ExitGames::Common::JString& retStr, bool withTypes) const {
		return retStr =
			ExitGames::Common::JString(L"<") +
			(withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
				: L"") +
			frame_nbr + L">" +
			(withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
				: L"") +
			input + L">";
	}

	bool FrameInput::compare(const CustomTypeBase& other) const {
		return frame_nbr == dynamic_cast<const FrameInput&>(other).frame_nbr &&
			input == dynamic_cast<const FrameInput&>(other).input;
	}

	void FrameInput::deserialize(const nByte* pData, short length) {
		ExitGames::Common::Deserializer d(pData, length, serialization_protocol);
		ExitGames::Common::Object o{};

		d.pop(o);
		frame_nbr = ExitGames::Common::ValueObject<short>(o).getDataCopy();

		d.pop(o);
		input = ExitGames::Common::ValueObject<uint8_t>(o).getDataCopy();
	}

	short FrameInput::serialize(nByte* pRetVal) const {
		ExitGames::Common::Serializer s(serialization_protocol);

		s.push(frame_nbr);
		s.push(input);

		if (pRetVal) {
			MEMCPY(pRetVal, s.getData(), s.getSize());
		}

		return static_cast<short>(s.getSize());
	}

	void FrameInput::duplicate(CustomTypeBase* pRetVal) const {
		*reinterpret_cast<FrameInput*>(pRetVal) = *this;
	}

	bool FrameInput::operator==(const FrameInput& other) const noexcept {
		return  frame_nbr == other.frame_nbr && input == other.input;
	}

}
