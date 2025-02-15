#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "DllTemplate.h"

namespace Kita {

    class KITAENGINE_API Logger {
    public:
        static void init();
        static std::shared_ptr<spdlog::logger>& getEngineInstance();
        static std::shared_ptr<spdlog::logger>& getUserInstance();
    private:
        static inline std::shared_ptr<spdlog::logger> m_engineInstance = nullptr;
        static inline std::shared_ptr<spdlog::logger> m_userInstance = nullptr;
    };

} // Kita

#define KITA_ENGINE_DEBUG(...) ::Kita::Logger::getEngineInstance()->debug(__VA_ARGS__)
#define KITA_ENGINE_INFO(...) ::Kita::Logger::getEngineInstance()->info(__VA_ARGS__)
#define KITA_ENGINE_WARN(...) ::Kita::Logger::getEngineInstance()->warn(__VA_ARGS__)
#define KITA_ENGINE_ERROR(...) ::Kita::Logger::getEngineInstance()->error(__VA_ARGS__)

#define KITA_DEBUG(...) ::Kita::Logger::getUserInstance()->debug(__VA_ARGS__)
#define KITA_INFO(...) ::Kita::Logger::getUserInstance()->info(__VA_ARGS__)
#define KITA_WARN(...) ::Kita::Logger::getUserInstance()->warn(__VA_ARGS__)
#define KITA_ERROR(...) ::Kita::Logger::getUserInstance()->error(__VA_ARGS__)