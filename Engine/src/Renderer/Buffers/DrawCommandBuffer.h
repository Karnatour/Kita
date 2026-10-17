#pragma once
#include "../../Core/DllTemplate.h"
#include "../Properties/DrawCommandProperties.h"

namespace Kita {
    class KITAENGINE_API DrawCommandBuffer {
    public:
        enum class Type {
            ARRAYS,
            ELEMENTS
        };

        DrawCommandBuffer() = default;
        DrawCommandBuffer(const DrawCommandBuffer&) = delete;
        DrawCommandBuffer& operator=(const DrawCommandBuffer&) = delete;
        DrawCommandBuffer(DrawCommandBuffer&&) = delete;
        DrawCommandBuffer& operator=(DrawCommandBuffer&&) = delete;
        virtual ~DrawCommandBuffer() = default;

        static std::unique_ptr<DrawCommandBuffer> createPtr();

        virtual void createEmptyBuffer(Type type, size_t size) = 0;
        virtual void createBuffer(const std::vector<DrawArraysIndirectCommandProperties>& arraysCommands) = 0;
        virtual void createBuffer(const std::vector<DrawElementsIndirectCommandProperties>& elementsCommands) = 0;
        virtual void upload(size_t offset, const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) = 0;
        virtual void upload(size_t offset, const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) = 0;
        virtual void bind() = 0;

        unsigned int getDCB() const;
        Type getType() const;

    protected:
        unsigned int m_dcb = 0;
        Type m_type;
    };
} // Kita
