#include "Timer.h"

void Physics::Timer::OnStart() noexcept
{
    _startTime = std::chrono::high_resolution_clock::now();
}

[[nodiscard]] float Physics::Timer::DeltaTime() noexcept
{
    const auto _currentTime = std::chrono::high_resolution_clock::now();
    const float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(
            (_currentTime - _startTime)).count();
    _startTime = _currentTime;
    return deltaTime;
}