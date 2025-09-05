#pragma once
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API FrameBuffer {
    public:
        virtual ~FrameBuffer() = default;
        virtual void createBuffer(const std::pair<int, int>& resolution, unsigned int colorAsRenderBuffer, unsigned int depthAsRenderBuffer, unsigned int stencilAsRenderBuffer) = 0;
        virtual void attachCubemapFace(unsigned int cubemapTexture, int faceIndex) = 0;
        unsigned int getFBO() const;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        static std::shared_ptr<FrameBuffer> createPtr();
        std::pair<int, int> getResolution() const;

    protected:
        unsigned int m_fbo = 0;
        unsigned int m_colorTexture = 0;
        unsigned int m_depthTexture = 0;
        unsigned int m_stencilTexture = 0;
        std::pair<int, int> m_resolution;
    };
} // Kita
