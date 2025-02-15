#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Kita {
    void Logger::init() {
        m_engineInstance = spdlog::stdout_color_mt("Engine");
        m_userInstance = spdlog::stdout_color_mt("User");
        spdlog::set_pattern("[%n|%H:%M:%S] %^%v%$");
#ifdef KITA_BUILD_DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
        KITA_ENGINE_INFO("Logger initialized");
    }

    std::shared_ptr<spdlog::logger>& Logger::getEngineInstance() {
        return m_engineInstance;
    }

    std::shared_ptr<spdlog::logger>& Logger::getUserInstance() {
        return m_userInstance;
    }
} // Kita
