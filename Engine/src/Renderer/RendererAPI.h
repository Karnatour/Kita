#pragma once
#include <complex>

#include "../Core/DllTemplate.h"
#include "Types/RenderingAPI.h"
#include <memory>

#include "Scene/ECS/Entity.h"
#include "Scene/Entities/LightUtil.h"
#include "Scene/Entities/SkyboxEntity.h"
#include "../Events/WindowEvents.h"

namespace Kita {
    class Renderer;

    class KITAENGINE_API RendererAPI {
    public:
        virtual ~RendererAPI() = default;
        void init(const Renderer& renderer);
        static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);
        virtual void setupDebug() = 0;

        //RENDERR virtual void setViewport(int width, int height, bool rewriteStoredPair) = 0;
        //RENDRER virtual void restoreViewport() = 0;

        //void renderShadowPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::vector<std::shared_ptr<LightUtil>>& lightEntities);
        //void renderMainPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<SkyboxEntity>& skyboxEntity);
        // RENDERER void renderMainFrameBufferToScreen();

        //void renderEntity(const std::shared_ptr<Entity>& entity);
        // RENDERER void renderMesh(const std::shared_ptr<Mesh>& mesh);

        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearBit(std::span<const ClearBit> bits) = 0;
        virtual void enableCapability(Capability capability) = 0;
        virtual void disableCapability(Capability capability) = 0;
        virtual void enableBufferWrite(BufferType bufferType) = 0;
        virtual void disableBufferWrite(BufferType bufferType) = 0;
        virtual void setDepthFunc(DepthFunction function) = 0;

        virtual void drawArrays(size_t verticesCount) = 0;
        virtual void drawElements(size_t indicesCount) = 0;

    private:
        //void renderSkyboxEntity(const std::shared_ptr<SkyboxEntity>& skyboxEntity);
        void prepareScreenQuadModel(const Renderer& renderer);
        //void enableTextureInShader(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture);
        //void setMaterial(int materialIndex, const std::vector<std::shared_ptr<Material>>& materials, const Transformation& transformation);
        //void setMaterial(int materialIndex, const std::vector<std::shared_ptr<Material>>& materials);
        static void onFrameBufferResize(const FrameBufferResized& event);
        std::unique_ptr<FrameBuffer> m_mainFramebuffer = FrameBuffer::createPtr();
        //std::unique_ptr<Model> m_screenQuadModel;
    };
} // Kita
