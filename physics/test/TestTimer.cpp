#include "Timer.h"
#include "gtest/gtest.h"

TEST(Timer, OnStart)
{
    Engine::Timer timer;
    timer.OnStart();

    auto firstStartTime = timer._startTime;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.OnStart();

    EXPECT_NE(firstStartTime, timer._startTime);
}

TEST(Timer, DeltaTime)
{
    Engine::Timer timer;
    timer.OnStart();

    std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::chrono::time_point<std::chrono::steady_clock> _currentTime = std::chrono::high_resolution_clock::now();

    float timerDeltaTime = timer.DeltaTime();
    float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>((_currentTime - startTime)).count();

    EXPECT_NEAR(timerDeltaTime, deltaTime, 0.0001);
}