#include "GLUniformBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLUniformBuffer::~GLUniformBuffer() {
        glDeleteBuffers(1,&m_ubo);
    }

    void GLUniformBuffer::createBuffer() {
        glCreateBuffers(1,&m_ubo);
        glNamedBufferStorage()
    }
} // Kita