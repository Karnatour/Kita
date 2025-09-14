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
        virtual void createBuffer(const std::pair<int, int>& resolution, std::initializer_list<AttachmentSpec> attachments) = 0;
        virtual void attachCubemapFace(unsigned int cubemapTexture, int faceIndex) = 0;
        unsigned int getFBO() const;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        static std::shared_ptr<FrameBuffer> createPtr();
        std::pair<int, int> getResolution() const;

        std::shared_ptr<Texture> getDepthTexture() const;
    protected:
        unsigned int m_fbo = 0;
        //Set to nullptr since we dont use all textures/RBOs always
        std::shared_ptr<Texture> m_colorTexture = nullptr;
        std::shared_ptr<Texture> m_depthTexture = nullptr;
        std::shared_ptr<Texture> m_stencilTexture = nullptr;

        std::shared_ptr<RenderBuffer> m_colorRenderBuffer = nullptr;
        std::shared_ptr<RenderBuffer> m_depthRenderBuffer = nullptr;
        std::shared_ptr<RenderBuffer> m_stencilRenderBuffer = nullptr;

        std::pair<int, int> m_resolution;
    };
} // Kita
