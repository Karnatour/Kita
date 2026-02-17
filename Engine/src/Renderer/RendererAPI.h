#pragma once
#include <complex>

#include "../Core/DllTemplate.h"
#include "Structs/RenderingAPI.h"
#include <memory>

#include "Scene/Entity.h"
#include "Scene/Entities/LightEntity.h"
#include "Scene/Entities/SkyboxEntity.h"
#include "../Events/WindowEvents.h"

namespace Kita {
    class Renderer;

    class KITAENGINE_API RendererAPI {
    public:
        virtual ~RendererAPI() = default;
        void init(Renderer& renderer);
        static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);
        virtual void setupDebug() = 0;

        virtual void setViewport(int width, int height, bool rewriteStoredPair) = 0;
        virtual void restoreViewport() = 0;

        void renderShadowPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::vector<std::shared_ptr<LightEntity>>& lightEntities);
        void renderMainPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<SkyboxEntity>& skyboxEntity);
        void renderMainFrameBufferToScreen();

        void renderEntity(const std::shared_ptr<Entity>& entity);
        void renderMesh(const std::shared_ptr<Mesh>& mesh);

        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearBit(std::initializer_list<ClearBit> bits) = 0;
        virtual void enableCapability(const Capability& capability) = 0;
        virtual void disableCapability(const Capability& capability) = 0;
        virtual void enableBufferWrite(const BufferType& bufferType) = 0;
        virtual void disableBufferWrite(const BufferType& bufferType) = 0;
        virtual void setDepthFunc(const DepthFunction& function) = 0;

        virtual void drawArrays(const size_t& verticesCount) = 0;
        virtual void drawElements(const size_t& indicesCount) = 0;

        std::pair<int, int> getViewport() const;

    protected:
        std::pair<int, int> m_viewport = std::make_pair(1600, 900);
    private:
        void renderSkyboxEntity(const std::shared_ptr<SkyboxEntity>& skyboxEntity);
        void prepareScreenQuadModel(Renderer& renderer);
        void enableTextureInShader(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture);
        void setMaterial(int materialIndex, const std::vector<std::shared_ptr<Material>>& materials, const Transformation& transformation);
        void setMaterial(int materialIndex, const std::vector<std::shared_ptr<Material>>& materials);
        static void onFrameBufferResize(const FrameBufferResized& event);
        std::unique_ptr<FrameBuffer> m_mainFramebuffer = FrameBuffer::createPtr();
        std::unique_ptr<Model> m_screenQuadModel;
    };
} // Kita
