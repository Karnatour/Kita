#include "../kitapch.h"
#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Kita {
    void Logger::init() {
        std::vector<spdlog::sink_ptr> engineSinks = {
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine.log", true)
        };

        std::vector<spdlog::sink_ptr> userSinks = {
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/user.log", true)
        };

        m_engineInstance = std::make_shared<spdlog::logger>("Engine", engineSinks.begin(), engineSinks.end());
#ifdef KITA_BUILD_DEBUG
        m_engineInstance->set_level(spdlog::level::debug);
        m_engineInstance->flush_on(spdlog::level::debug);
#endif
        m_engineInstance->set_pattern("[%n|%H:%M:%S] %^%v%$");

        m_userInstance = std::make_shared<spdlog::logger>("User", userSinks.begin(), userSinks.end());
#ifdef KITA_BUILD_DEBUG
        m_userInstance->set_level(spdlog::level::debug);
        m_engineInstance->flush_on(spdlog::level::debug);
#endif
        m_userInstance->set_pattern("[%n|%H:%M:%S] %^%v%$");

        spdlog::register_logger(m_engineInstance);
        spdlog::register_logger(m_userInstance);
        KITA_ENGINE_INFO("Logger initialized");
    }

    const std::shared_ptr<spdlog::logger>& Logger::getEngineInstance() {
        return m_engineInstance;
    }

    const std::shared_ptr<spdlog::logger>& Logger::getUserInstance() {
        return m_userInstance;
    }
} // Kita
