#include "../../../kitapch.h"
#include "GLElementBuffer.h"

#include <glad/glad.h>

namespace Kita {
    void GLElementBuffer::create(std::vector<unsigned int>& indicies) {
        glCreateBuffers(1, &m_ebo);
        glNamedBufferStorage(m_ebo, static_cast<GLsizeiptr>(sizeof(unsigned int) * indicies.size()), indicies.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
