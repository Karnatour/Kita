#pragma once
#include "../Entity.h"
#include "../../../Core/DllTemplate.h"
#include "../../Buffers/FrameBuffer.h"
#include "../../Buffers/RenderBuffer.h"
#include "../../Textures/Texture.h"

namespace Kita {
    class KITAENGINE_API SkyboxEntity final : public Entity {
    public:
        explicit SkyboxEntity(const std::filesystem::path& texturePath);
        bool onRender(RendererAPI& rendererApi) override;
    private:
        std::shared_ptr<RenderBuffer> m_depthRenderBuffer = RenderBuffer::createPtr();
        std::shared_ptr<RenderBuffer> m_stencilRenderBuffer = RenderBuffer::createPtr();
        std::shared_ptr<FrameBuffer> m_frameBuffer = FrameBuffer::createPtr();

        std::shared_ptr<Texture> m_skyboxTexture = Texture::createPtr();
        std::shared_ptr<Texture> m_cubemapTexture = Texture::createPtr();

        glm::mat4 m_captureProjection;
        glm::mat4 m_captureViews[6];
    };
} // Kita