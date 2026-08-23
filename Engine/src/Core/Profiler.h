#pragma once
#include "DllTemplate.h"
#include <tracy/Tracy.hpp>

namespace Kita {
}

#if defined(KITA_TRACY_ENABLE)
#define KITA_ENGINE_PROFILE(label) ZoneScopedN(label)
#define KITA_ENGINE_PROFILE_FRAME_MARK FrameMark
#else
#define KITA_ENGINE_PROFILE(label) ((void)0)
#define KITA_ENGINE_PROFILE_FRAME_MARK ((void)0)
#endif
