#pragma once
#include <Common-cpp/inc/CustomType.h>
#include <cstdint>
#include "Vec2.h"


namespace Input {
	constexpr std::uint8_t kJump = 1 << 0;
	constexpr std::uint8_t kRight = 1 << 1;
	constexpr std::uint8_t kLeft = 1 << 2;
	constexpr std::uint8_t kAttack = 1 << 3;



	class FrameInput final : public ExitGames::Common::CustomType<FrameInput, 1> {
		typedef CustomType<FrameInput, 1> super;

	public:
		FrameInput() noexcept = default;
		FrameInput(Math::Vec2F dir_to_mouse, short frame_nbr,
			std::uint8_t input) noexcept;
		FrameInput(FrameInput&& toMove) noexcept = default;
		FrameInput& operator=(FrameInput&& toMove) noexcept = default;
		FrameInput(const FrameInput& toCopy) noexcept = default;
		FrameInput& operator=(const FrameInput& toCopy) noexcept = default;
		~FrameInput() override = default;

		ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr,
			bool withTypes) const override;
		bool compare(const CustomTypeBase& other) const override;
		void deserialize(const nByte* pData, short length) override;
		short serialize(nByte* pRetVal) const override;
		void duplicate(CustomTypeBase* pRetVal) const override;

		bool operator==(const FrameInput& other) const noexcept;

		void UpdatePlayerInputs();


		short frame_nbr = 0;
		std::uint8_t input = 0;

		static nByte serialization_protocol;
	};
}  // namespace Input
