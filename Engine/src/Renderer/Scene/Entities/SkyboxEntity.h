#pragma once
#include "../Entity.h"
#include "../../../Core/DllTemplate.h"
#include "../../Buffers/FrameBuffer.h"
#include "../../Textures/Texture.h"

namespace Kita {
    class KITAENGINE_API SkyboxEntity final : public Entity {
    public:
        explicit SkyboxEntity(const std::filesystem::path& texturePath);
        bool onRender(RendererAPI& rendererApi) override;

    private:
        void setupCubemapViews();
        void prepareSkyboxModel();
        void renderToFramebuffer(RendererAPI& rendererApi);
        void swapToFinalSkyboxMaterial();
        std::shared_ptr<FrameBuffer> m_frameBuffer = FrameBuffer::createPtr();

        std::shared_ptr<Texture> m_skyboxTexture = Texture::createPtr();
        std::shared_ptr<Texture> m_cubemapTexture = Texture::createPtr();

        glm::mat4 m_captureProjection{};
        std::vector<glm::mat4> m_captureViews;
    };
} // Kita