#pragma once

#include <chrono>

namespace Engine
{
    /**
     * @class Timer
     * @brief Represents a _timer utility for measuring elapsed time and calculating delta time.
     * The Timer class provides functionality to track the elapsed time since its initiation
     * and calculate the time difference between subsequent calls, commonly known as delta time.
     * The class utilizes the high-resolution clock from the C++ chrono library for accurate time measurements.
     */
    class Timer
    {
    public :
        std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;

        /**
        * @brief Set the _startTime to the moment we called it.
        */
        void OnStart() noexcept;

        /**
        * @brief Calculate the delta time with _startTime and the currentTime,
        * \n Note : _startTime need to be instantiate with OnStart
        */
        [[nodiscard]] float DeltaTime() noexcept;
    };
}