#pragma once
#include "DllTemplate.h"
#include "Logger.h"
#include <cstdlib>
#include <stacktrace>

namespace Kita {
}

#ifdef KITA_BUILD_DEBUG
    #define KITA_ENGINE_ASSERT(condition, message) \
        if (!condition) { \
            KITA_ENGINE_ERROR("Assert error: {}\nStacktrace:\n{}", message, std::to_string(std::stacktrace::current())); \
            std::abort(); \
        }
#else
    #define KITA_ENGINE_ASSERT(condition, message) ((void)0)
#endif
