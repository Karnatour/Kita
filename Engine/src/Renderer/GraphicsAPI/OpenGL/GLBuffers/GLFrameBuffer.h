#pragma once
#include "../../../Buffers/FrameBuffer.h"

namespace Kita {
    class GLFrameBuffer final : public FrameBuffer {
    public:
        ~GLFrameBuffer() override;
        void createBuffer(const std::pair<int, int>& resolution, std::initializer_list<AttachmentSpec> attachments) override;
        void bind() override;
        void unbind() override;
        void attachCubemapFace(unsigned cubemapTexture, int faceIndex) override;

    private:
        void createAndAttachRenderBuffer(BufferType bufferType);
        void createAndAttachTexture(BufferType bufferType);
    };
} // Kita
