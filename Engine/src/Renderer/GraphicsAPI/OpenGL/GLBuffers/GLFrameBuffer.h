#pragma once
#include "../../../Buffers/FrameBuffer.h"
#include "../../../Buffers/BufferType.h"

namespace Kita {
    class GLFrameBuffer final : public FrameBuffer {
    public:
        ~GLFrameBuffer() override;
        void createBuffer(const std::pair<int, int>& resolution, unsigned int colorAsRenderBuffer, unsigned int depthAsRenderBuffer, unsigned int stencilAsRenderBuffer) override;
        void bind() override;
        void unbind() override;
        void attachCubemapFace(unsigned cubemapTexture, int faceIndex) override;

    private:
        void createAndAttachTextureToFBO(unsigned int& texture, const BufferType& type);
    };
} // Kita
