#pragma once
#include <glad/glad.h>

#include "../../DebugScope.h"

namespace Kita {
    class GLDebugScope : public DebugScope {
    public:
        ~GLDebugScope() override {
#ifdef KITA_BUILD_DEBUG
            glPopDebugGroup();
#endif
        }

        void scope(const std::string_view name) override {
#ifdef KITA_BUILD_DEBUG
            glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, static_cast<GLsizei>(name.size()), name.data());
#endif
        };
    };
} // Kita
