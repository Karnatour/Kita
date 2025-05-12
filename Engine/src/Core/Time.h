#pragma once
#include "DllTemplate.h"
#include <chrono>

namespace Kita {
    class KITAENGINE_API Time {
    public:
        static void updateDeltaTime(std::chrono::time_point<std::chrono::system_clock> currentFrameTime);
        static double getDeltaTime();
        static void setLastFrameTime(std::chrono::time_point<std::chrono::system_clock> lastFrameTime);
        static std::chrono::time_point<std::chrono::system_clock> getLastFrameTime();
        static double getElapsedTime();

    private:
        inline static std::chrono::duration<double> m_deltaTime;
        inline static std::chrono::time_point<std::chrono::system_clock> m_lastFrameTime;
        inline static std::chrono::time_point<std::chrono::system_clock> m_startTime = std::chrono::system_clock::now();
    };
} // Kita
