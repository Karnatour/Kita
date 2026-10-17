#include "GLDrawCommandBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLDrawCommandBuffer::~GLDrawCommandBuffer() {
        if (m_dcb != 0) {
            glDeleteBuffers(1, &m_dcb);
            m_dcb = 0;
        }
    }

    void GLDrawCommandBuffer::createEmptyBuffer(const Type type, const size_t size) {
        KITA_ENGINE_ASSERT(m_dcb == 0, "Failed to create empty buffer since DrawCommandBuffer was already created");
        m_type = type;
        glCreateBuffers(1, &m_dcb);
        glNamedBufferStorage(m_dcb, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    }

    void GLDrawCommandBuffer::createBuffer(const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) {
        KITA_ENGINE_ASSERT(m_dcb == 0, "DrawCommandBuffer already exist on GPU create a new one in new object");
        m_type = Type::ARRAYS;
        glCreateBuffers(1, &m_dcb);
        glNamedBufferStorage(m_dcb, sizeof(DrawArraysIndirectCommandProperties) * arraysIndirectCommands.size(), arraysIndirectCommands.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    void GLDrawCommandBuffer::createBuffer(const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) {
        KITA_ENGINE_ASSERT(m_dcb == 0, "DrawCommandBuffer already exist on GPU create a new one in new object");
        m_type = Type::ELEMENTS;
        glCreateBuffers(1, &m_dcb);
        glNamedBufferStorage(m_dcb, sizeof(DrawElementsIndirectCommandProperties) * elementsIndirectCommands.size(), elementsIndirectCommands.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    void GLDrawCommandBuffer::upload(const size_t offset, const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) {
        KITA_ENGINE_ASSERT(m_dcb != 0, "DrawCommandBuffer has not been created, call createBuffer first");
        glNamedBufferSubData(m_dcb, offset, sizeof(DrawArraysIndirectCommandProperties) * arraysIndirectCommands.size(), arraysIndirectCommands.data());
    }

    void GLDrawCommandBuffer::upload(const size_t offset, const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) {
        KITA_ENGINE_ASSERT(m_dcb != 0, "DrawCommandBuffer has not been created, call createBuffer first");
        glNamedBufferSubData(m_dcb, offset, sizeof(DrawElementsIndirectCommandProperties) * elementsIndirectCommands.size(), elementsIndirectCommands.data());
    }


    void GLDrawCommandBuffer::bind() {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_dcb);
    }
} // Kita
