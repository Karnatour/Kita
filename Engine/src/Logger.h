#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "DllTemplate.h"

namespace Kita {

    class KITAENGINE_API Logger {
    public:
        static void init();
        static std::shared_ptr<spdlog::logger>& getEngineLogger();
        static std::shared_ptr<spdlog::logger>& getUserLogger();
    private:
        static std::shared_ptr<spdlog::logger> engineLogger;
        static std::shared_ptr<spdlog::logger> userLogger;
    };
} // Kita
