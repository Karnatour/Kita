#pragma once
#include "../../Core/DllTemplate.h"
#include "../Types/RendererTypes.h"
#include <utility>
#include <memory>

namespace Kita {
    class KITAENGINE_API RenderBuffer {
    public:
        RenderBuffer() = default;
        RenderBuffer(const RenderBuffer&) = delete;
        RenderBuffer& operator=(const RenderBuffer&) = delete;
        RenderBuffer(RenderBuffer&&) = delete;
        RenderBuffer& operator=(RenderBuffer&&) = delete;
        virtual ~RenderBuffer() = default;

        [[nodiscard]] static std::unique_ptr<RenderBuffer> createPtr();

        virtual void createBuffer(const std::pair<int, int>& resolution, BufferType bufferType) = 0;
        virtual void bind() = 0;
        virtual void destroy() = 0;

        unsigned int getRBO() const;
        std::pair<int, int> getResolution() const;

    protected:
        unsigned int m_rbo = 0;
        std::pair<int, int> m_resolution;
    };
} // Kita
