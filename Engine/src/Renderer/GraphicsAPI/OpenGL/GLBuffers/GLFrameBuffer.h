#pragma once
#include "../../../Buffers/FrameBuffer.h"

namespace Kita {
    class GLFrameBuffer final : public FrameBuffer {
    public:
        GLFrameBuffer(const std::optional<std::pair<int, int>>& resolution, std::initializer_list<AttachInfo> attachments, bool highPrecision, int layers);
        ~GLFrameBuffer() override;
        void bind() override;
        void unbind() override;
        void attachCubemapFace(const Texture& cubemapTexture, int faceIndex) override;
        void attachExternalTexture(BufferType bufferType, const Texture& texture) override;
        void resize(const std::pair<int, int>& newResolution) override;

    private:
        void createAndAttachAttachments();
        void createAndAttachRenderBuffer(BufferType bufferType);
        void createAndAttachTexture(BufferType bufferType, bool highPrecision);
        void destroyAttachments();
        void verifyBuild();
    };
} // Kita
