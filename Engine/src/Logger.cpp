#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Kita {
    std::shared_ptr<spdlog::logger> Logger::engineLogger;
    std::shared_ptr<spdlog::logger> Logger::userLogger;

    void Logger::init() {
        engineLogger = spdlog::stdout_color_mt("Engine");
        userLogger = spdlog::stdout_color_mt("User");
        spdlog::set_pattern("[%n|%H:%M:%S] %^%v%$");
#ifdef KITA_DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
    }

    std::shared_ptr<spdlog::logger>& Logger::getEngineLogger() {
        return engineLogger;
    }

    std::shared_ptr<spdlog::logger>& Logger::getUserLogger() {
        return userLogger;
    }
} // Kita
