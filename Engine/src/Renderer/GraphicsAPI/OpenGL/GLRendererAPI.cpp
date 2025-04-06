#include "../../../kitapch.h"
#include "GLRendererAPI.h"

#include <glad/glad.h>

namespace Kita {
    void GLRendererAPI::setViewport(const int width, const int height) {
        glViewport(0, 0, width, height);
    }
} // Kita