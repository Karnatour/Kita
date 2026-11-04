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

        DrawCommandBuffer(const DrawCommandBuffer&) = delete;
        DrawCommandBuffer& operator=(const DrawCommandBuffer&) = delete;
        DrawCommandBuffer(DrawCommandBuffer&&) = delete;
        DrawCommandBuffer& operator=(DrawCommandBuffer&&) = delete;
        virtual ~DrawCommandBuffer() = default;

        [[nodiscard]] static std::unique_ptr<DrawCommandBuffer> createPtr(const std::vector<DrawArraysIndirectCommandProperties>& arraysCommands);
        [[nodiscard]] static std::unique_ptr<DrawCommandBuffer> createPtr(const std::vector<DrawElementsIndirectCommandProperties>& elementsCommands);

        virtual void upload(size_t offset, const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) = 0;
        virtual void upload(size_t offset, const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) = 0;
        virtual void bind() = 0;

        unsigned int getDCB() const;
        Type getType() const;

    protected:
        DrawCommandBuffer(Type type);
        unsigned int m_dcb = 0;
        Type m_type;
    };
} // Kita
