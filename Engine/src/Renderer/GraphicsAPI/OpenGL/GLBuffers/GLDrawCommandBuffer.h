#pragma once
#include "../../../Buffers/DrawCommandBuffer.h"

namespace Kita {
    class GLDrawCommandBuffer final : public DrawCommandBuffer {
    public:
        explicit GLDrawCommandBuffer(const std::vector<DrawArraysIndirectCommandProperties>& arraysCommands);
        explicit GLDrawCommandBuffer(const std::vector<DrawElementsIndirectCommandProperties>& elementsCommands);
        ~GLDrawCommandBuffer() override;
        void upload(size_t offset, const std::vector<DrawArraysIndirectCommandProperties>& arraysIndirectCommands) override;
        void upload(size_t offset, const std::vector<DrawElementsIndirectCommandProperties>& elementsIndirectCommands) override;
        void bind() override;
    };
} // Kita
