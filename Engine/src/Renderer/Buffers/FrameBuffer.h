#pragma once
#include "RenderBuffer.h"
#include "../../Core/DllTemplate.h"
#include "../Textures/Texture.h"

namespace Kita {
    class KITAENGINE_API FrameBuffer {
    public:
        enum class AttachmentType {
            RENDERBUFFER,
            TEXTURE
        };

        struct AttachmentSpec {
            BufferType bufferType;
            AttachmentType attachmentType;
        };

        virtual ~FrameBuffer() = default;
        virtual void createBuffer(const std::pair<int, int>& resolution, std::initializer_list<AttachmentSpec> attachments, bool highPrecision) = 0;
        virtual void attachCubemapFace(unsigned int cubemapTexture, int faceIndex) = 0;
        unsigned int getFBO() const;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void resize(const std::pair<int,int>& newResolution) = 0;
        static std::unique_ptr<FrameBuffer> createPtr();
        std::pair<int, int> getResolution() const;

        std::shared_ptr<Texture> getColorTexture() const;
        std::shared_ptr<Texture> getDepthTexture() const;

        const RenderBuffer* getColorRenderBuffer() const;
        const RenderBuffer* getDepthRenderBuffer() const;
    protected:
        unsigned int m_fbo = 0;
        std::shared_ptr<Texture> m_colorTexture;
        std::shared_ptr<Texture> m_depthTexture;

        std::unique_ptr<RenderBuffer> m_colorRenderBuffer;
        std::unique_ptr<RenderBuffer> m_depthRenderBuffer;

        std::pair<int, int> m_resolution;

        std::vector<AttachmentSpec> m_attachments;
        bool m_highPrecision = false;
    };
} // Kita
