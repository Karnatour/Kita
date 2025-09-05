#pragma once
#include <glad/glad.h>

#include "../../../Buffers/RenderBuffer.h"

namespace Kita {
    class GLRenderBuffer final : public RenderBuffer {
    public:
        ~GLRenderBuffer() override;
        void createBuffer(const std::pair<int, int>& resolution, const BufferType& bufferType) override;
        void bind() override;

    private:
        GLenum convertBufferTypeToInternalFormat(const BufferType& buffer);
    };
} // Kita
