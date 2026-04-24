#pragma once
#include "../Core/DllTemplate.h"
#include "Types/RenderingAPI.h"
#include <memory>

#include "Scene/ECS/Entity.h"
#include "Scene/Entities/SkyboxEntity.h"

namespace Kita {
    class KITAENGINE_API RendererAPI {
    public:
        virtual ~RendererAPI() = default;
        static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);

    private:
        friend class Renderer;

        void init(const Renderer& renderer);
        virtual void setupDebug() = 0;

        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearBit(std::span<const ClearBit> bits) = 0;
        virtual void enableCapability(Capability capability) = 0;
        virtual void disableCapability(Capability capability) = 0;
        virtual void enableBufferWrite(BufferType bufferType) = 0;
        virtual void disableBufferWrite(BufferType bufferType) = 0;
        virtual void setDepthFunc(DepthFunction function) = 0;

        virtual void drawArrays(size_t verticesCount) = 0;
        virtual void drawElements(size_t indicesCount) = 0;

        virtual void setViewport(std::pair<int, int> resolution) = 0;
        void prepareScreenQuadModel(const Renderer& renderer);
    };
} // Kita
