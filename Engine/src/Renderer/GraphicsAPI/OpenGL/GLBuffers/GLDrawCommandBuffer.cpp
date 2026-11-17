#include "GLDrawCommandBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLDrawCommandBuffer::GLDrawCommandBuffer(const std::vector<DrawArraysIndirectCommandProperties>& arraysCommands) : DrawCommandBuffer(Type::ARRAYS) {
        KITA_ENGINE_ASSERT(!arraysCommands.empty(), "DrawCommandBuffer has received empty commands vector");

        glCreateBuffers(1, &m_dcb);
        glNamedBufferStorage(m_dcb, sizeof(DrawArraysIndirectCommandProperties) * arraysCommands.size(), arraysCommands.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    GLDrawCommandBuffer::GLDrawCommandBuffer(const std::vector<DrawElementsIndirectCommandProperties>& elementsCommands) : DrawCommandBuffer(Type::ELEMENTS) {
        KITA_ENGINE_ASSERT(!elementsCommands.empty(), "DrawCommandBuffer has received empty commands vector");

        glCreateBuffers(1, &m_dcb);
        glNamedBufferStorage(m_dcb, sizeof(DrawElementsIndirectCommandProperties) * elementsCommands.size(), elementsCommands.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    GLDrawCommandBuffer::~GLDrawCommandBuffer() {
        if (m_dcb != 0) {
            glDeleteBuffers(1, &m_dcb);
            m_dcb = 0;
        }
    }

    void GLDrawCommandBuffer::upload(const size_t offset, const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) {
        glNamedBufferSubData(m_dcb, offset, sizeof(DrawArraysIndirectCommandProperties) * arraysIndirectCommands.size(), arraysIndirectCommands.data());
    }

    void GLDrawCommandBuffer::upload(const size_t offset, const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) {
        glNamedBufferSubData(m_dcb, offset, sizeof(DrawElementsIndirectCommandProperties) * elementsIndirectCommands.size(), elementsIndirectCommands.data());
    }

    void GLDrawCommandBuffer::bind() {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_dcb);
    }
} // Kita
