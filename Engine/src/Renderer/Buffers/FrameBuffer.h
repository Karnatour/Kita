#pragma once
#include "RenderBuffer.h"
#include "../../Core/DllTemplate.h"
#include "../../Assets/Texture.h"

namespace Kita {
    class KITAENGINE_API FrameBuffer {
    public:
        enum class AttachType {
            RENDERBUFFER,
            TEXTURE
        };

        struct AttachInfo {
            BufferType bufferType;
            AttachType attachmentType;
        };

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&&) = delete;
        FrameBuffer& operator=(FrameBuffer&&) = delete;
        virtual ~FrameBuffer() = default;

        [[nodiscard]] static std::unique_ptr<FrameBuffer> createPtr(const std::optional<std::pair<int, int>>& resolution, std::initializer_list<AttachInfo> attachments, bool highPrecision = false, int layers = 0);

        virtual void attachCubemapFace(const Texture& cubemapTexture, int faceIndex) = 0;
        virtual void attachExternalTexture(BufferType type, const Texture& texture) = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void resize(const std::pair<int, int>& newResolution) = 0;

        unsigned int getFBO() const;
        std::pair<int, int> getResolution() const;

        Texture* getColorTexture() const;
        Texture* getDepthTexture() const;

        RenderBuffer* getColorRenderBuffer() const;
        RenderBuffer* getDepthRenderBuffer() const;

    protected:
        FrameBuffer(const std::optional<std::pair<int, int>>& resolution, std::initializer_list<AttachInfo> attachments, bool highPrecision, int layers);

        unsigned int m_fbo = 0;

        std::unique_ptr<Texture> m_colorTexture;
        std::unique_ptr<Texture> m_depthTexture;

        std::unique_ptr<RenderBuffer> m_colorRenderBuffer;
        std::unique_ptr<RenderBuffer> m_depthRenderBuffer;

        std::pair<int, int> m_resolution = {-1, -1};

        std::vector<AttachInfo> m_attachments;
        bool m_highPrecision = false;
        int m_layers = 0;
    };
} // Kita
