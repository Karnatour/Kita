#pragma once
#include <complex>

#include "../Core/DllTemplate.h"
#include "Structs/RenderingAPI.h"
#include <memory>

#include "Scene/Entity.h"
#include "Scene/Entities/LightEntity.h"
#include "Scene/Entities/SkyboxEntity.h"

namespace Kita {
    class KITAENGINE_API RendererAPI {
    public:
        virtual ~RendererAPI() = default;
        static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);
        virtual void setupDebug() = 0;

        virtual void setViewport(int width, int height, bool rewriteStoredPair) = 0;
        virtual void restoreViewport() = 0;

        void renderShadowPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::vector<std::shared_ptr<LightEntity>>& lightEntities);
        void renderMainPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<SkyboxEntity>& skyboxEntity);

        void renderEntity(const std::shared_ptr<Entity>& entity);
        void renderMesh(const std::shared_ptr<Mesh>& mesh);

        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clearBit(std::initializer_list<ClearBit> bits) = 0;
        virtual void enableCapability(const Capabilities& capability) = 0;
        virtual void disableCapability(const Capabilities& capability) = 0;
        virtual void enableBufferWrite(const BufferType& bufferType) = 0;
        virtual void disableBufferWrite(const BufferType& bufferType) = 0;
        virtual void setDepthFunc(const DepthFunctions& function) = 0;

        virtual void drawArrays(const size_t& verticesCount) = 0;
        virtual void drawElements(const size_t& indicesCount) = 0;

        std::pair<int, int> getViewport() const;

    protected:
        std::pair<int, int> m_viewport = std::make_pair(1600, 900);
    private:
        void renderSkyboxEntity(const std::shared_ptr<SkyboxEntity>& skyboxEntity);
        static void enableTextureInShader(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture);
        static void setMaterial(int materialIndex, const std::vector<std::shared_ptr<Material>>& materials, const Transformation& transformation);
    };
} // Kita
