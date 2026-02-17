#pragma once
#include "../../Core/DllTemplate.h"
#include "../../Renderer/Structs/RendererTypes.h"

namespace Kita {
    class KITAENGINE_API RenderBuffer {
    public:
        virtual ~RenderBuffer() = default;
        virtual void createBuffer(const std::pair<int, int>& resolution, BufferType bufferType) = 0;
        unsigned int getRBO() const;
        virtual void bind() = 0;
        virtual void destroy() = 0;
        static std::unique_ptr<RenderBuffer> createPtr();
        std::pair<int, int> getResolution() const;

    protected:
        unsigned int m_rbo = 0;
        std::pair<int, int> m_resolution;
    };
} // Kita
