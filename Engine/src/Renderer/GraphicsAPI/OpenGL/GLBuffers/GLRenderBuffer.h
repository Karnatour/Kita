#pragma once
#include "../../../Buffers/RenderBuffer.h"

namespace Kita {
    class GLRenderBuffer final : public RenderBuffer {
    public:
        GLRenderBuffer(const std::pair<int, int>& resolution, BufferType bufferType);
        ~GLRenderBuffer() override;
        void bind() override;
        void destroy() override;
    };
} // Kita
