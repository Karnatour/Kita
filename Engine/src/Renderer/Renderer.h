#pragma once

#include "Types/RenderingAPI.h"
#include "../Core/DllTemplate.h"
#include <memory>
#include "../Events/WindowEvents.h"
#include "RendererAPI.h"
#include "Buffers/FrameBuffer.h"

namespace Kita {
    class Texture;
    class Shader;
    class Mesh;

    class KITAENGINE_API Renderer {
    public:
        explicit Renderer(RenderingAPI api);
        static RenderingAPI getAPI();

        std::pair<int, int> getViewport() const;
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
        void setCullMode(CullMode mode);

        FrameBuffer& getOutputFramebuffer() const;
        FrameBuffer& getMainFramebuffer() const;
    private:
        void setMaterialInShader(Shader& shader, std::span<Texture* const> textures, const glm::mat4& modelMatrix);
        void setTexturesInShader(Shader& shader, std::span<Texture* const> textures);
        void resetTextureState(Shader& shader);

        static void onFrameBufferResize(const FrameBufferResized& event);
        std::unique_ptr<FrameBuffer> m_mainFramebuffer;
        std::unique_ptr<FrameBuffer> m_outputFramebuffer; // post process FBO, used by editor
        std::pair<int,int > m_viewport = std::make_pair(1600, 900);

        static inline RenderingAPI m_api = RenderingAPI::NONE;
        std::unique_ptr<RendererAPI> m_rendererAPI;
    };
} // Kita