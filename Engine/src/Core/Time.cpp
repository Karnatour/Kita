#include "Time.h"

namespace Kita {
    void Time::updateDeltaTime(const std::chrono::time_point<std::chrono::system_clock> currentFrameTime) {
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;
    }

    void Time::setLastFrameTime(const std::chrono::time_point<std::chrono::system_clock> lastFrameTime){
        m_lastFrameTime = lastFrameTime;
    }

    std::chrono::time_point<std::chrono::system_clock> Time::getLastFrameTime() {
        return m_lastFrameTime;
    }

    double Time::getDeltaTime() {
        return m_deltaTime.count();
    }
} // Kita