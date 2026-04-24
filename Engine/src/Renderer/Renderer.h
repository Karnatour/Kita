#pragma once

#include "Types/RenderingAPI.h"
#include "../Core/DllTemplate.h"
#include <memory>
#include <glm/mat4x4.hpp>
#include "../Events/WindowEvents.h"
#include "RendererAPI.h"

namespace Kita {
    class Shader;
    class Mesh;

    class KITAENGINE_API Renderer {
    public:
        explicit Renderer(RenderingAPI api);
        static RenderingAPI getAPI();

        std::pair<float, float> getViewport() const;
        void renderMesh(const Mesh& mesh, Shader& shader, const glm::mat4& modelMatrix, std::span<Texture* const> textures = {});
        void setViewport(std::pair<int, int> resolution, bool rewriteStoredPair);
        void restoreViewport();

        void clearColor(float red, float green, float blue, float alpha);
        void clearBit(std::span<const ClearBit> bits);
        void enableCapability(Capability capability);
        void disableCapability(Capability capability);
        void enableBufferWrite(BufferType bufferType);
        void disableBufferWrite(BufferType bufferType);
        void setDepthFunc(DepthFunction function);

        FrameBuffer& getMainFramebuffer() const;
    private:
        void setMaterialInShader(Shader& shader, std::span<Texture* const> textures, const glm::mat4& modelMatrix);
        void setTexturesInShader(Shader& shader, std::span<Texture* const> textures);
        void resetTextureState(Shader& shader);

        static void onFrameBufferResize(const FrameBufferResized& event);
        std::unique_ptr<FrameBuffer> m_mainFramebuffer = FrameBuffer::createPtr();
        std::pair<float, float> m_viewport = std::make_pair(1600.0f, 900.0f);

        static inline RenderingAPI m_api = RenderingAPI::NONE;
        std::unique_ptr<RendererAPI> m_rendererAPI;
    };
} // Kita