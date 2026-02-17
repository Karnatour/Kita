#include "Time.h"

namespace Kita {
    void Time::updateDeltaTime(const std::chrono::time_point<std::chrono::steady_clock> currentFrameTime) {
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;
    }

    void Time::setLastFrameTime(const std::chrono::time_point<std::chrono::steady_clock> lastFrameTime){
        m_lastFrameTime = lastFrameTime;
    }

    std::chrono::time_point<std::chrono::steady_clock> Time::getLastFrameTime() {
        return m_lastFrameTime;
    }

    double Time::getElapsedTime() {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsed = now - m_startTime;
        return elapsed.count();
    }

    double Time::getDeltaTime() {
        return m_deltaTime.count();
    }
} // Kita