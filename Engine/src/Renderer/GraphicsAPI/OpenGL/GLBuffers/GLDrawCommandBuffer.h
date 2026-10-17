#pragma once
#include "../../../Buffers/DrawCommandBuffer.h"

namespace Kita {
    class GLDrawCommandBuffer final : public DrawCommandBuffer {
    public:
        ~GLDrawCommandBuffer() override;
        void createEmptyBuffer(Type type, size_t size) override;
        void createBuffer(const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) override;
        void createBuffer(const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) override;
        void upload(size_t offset, const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) override;
        void upload(size_t offset, const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) override;
        void bind() override;
    };
} // Kita
