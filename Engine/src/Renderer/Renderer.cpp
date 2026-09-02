#include "../kitapch.h"
#include "Renderer.h"
#include "../Assets/Shader.h"
#include "../Core/Engine.h"
#include "../Assets/Mesh.h"
#include "../Events/EventManager.h"
#include "Buffers/FrameBuffer.h"

namespace Kita {
    Renderer::Renderer(const RenderingAPI api) {
        m_api = api;
        m_rendererAPI = RendererAPI::createPtr(api);
        m_rendererAPI->init();

        EventManager::listenToEvent<FrameBufferResized>(onFrameBufferResize);
        m_mainFramebuffer = FrameBuffer::createPtr();
        m_mainFramebuffer->createBuffer(
            m_viewport, {{{BufferType::COLOR, FrameBuffer::AttachType::TEXTURE}, {BufferType::DEPTH_STENCIL, FrameBuffer::AttachType::TEXTURE}}},
            true, 1);

        m_outputFramebuffer = FrameBuffer::createPtr();
        m_outputFramebuffer->createBuffer(m_viewport, {{{BufferType::COLOR, FrameBuffer::AttachType::TEXTURE}}}, true, 1);

        enableCapability(Capability::CULL_FACE);
        setCullMode(CullMode::BACK);
    }

    RenderingAPI Renderer::getAPI() {
        return m_api;
    }

    std::pair<int, int> Renderer::getViewport() const {
        return m_viewport;
    }

    void Renderer::renderMesh(const Mesh& mesh, Shader& shader, const glm::mat4& modelMatrix, const std::span<Texture* const> textures, const bool skipSetMaterial) {
        KITA_ENGINE_PROFILE("Render mesh");

        shader.bind();

        if (!skipSetMaterial) {
            setMaterialInShader(shader, textures);
        }
        shader.setUniformMat4("model", modelMatrix);

        mesh.getVertexArray().bind();
        if (mesh.getIndexBuffer()) {
            m_rendererAPI->drawElements(mesh.getIndexBuffer()->getIndices().size());
        } else {
            m_rendererAPI->drawArrays(mesh.getVertexBuffer().getVertices().size());
        }
    }

    void Renderer::onFrameBufferResize(const FrameBufferResized& event) {
        Engine::getEngine()->getRenderer().setViewport(event.getSize(), true);
        Engine::getEngine()->getRenderer().m_mainFramebuffer->resize(event.getSize());
        Engine::getEngine()->getRenderer().m_outputFramebuffer->resize(event.getSize());
    }

    void Renderer::setViewport(const std::pair<int, int> resolution, const bool rewriteStoredPair) {
        if (rewriteStoredPair) {
            m_viewport = resolution;
        }
        m_rendererAPI->setViewport(resolution);
    }

    void Renderer::restoreViewport() {
        m_rendererAPI->setViewport(m_viewport);
    }

    void Renderer::clearColor(const float red, const float green, const float blue, const float alpha) {
        m_rendererAPI->clearColor(red, green, blue, alpha);
    }

    void Renderer::clearBit(const std::span<const ClearBit> bits) {
        m_rendererAPI->clearBit(bits);
    }

    void Renderer::enableCapability(const Capability capability) {
        m_rendererAPI->enableCapability(capability);
    }

    void Renderer::disableCapability(const Capability capability) {
        m_rendererAPI->disableCapability(capability);
    }

    void Renderer::enableBufferWrite(const BufferType bufferType) {
        m_rendererAPI->enableBufferWrite(bufferType);
    }

    void Renderer::disableBufferWrite(const BufferType bufferType) {
        m_rendererAPI->disableBufferWrite(bufferType);
    }

    void Renderer::setDepthFunc(const DepthFunction function) {
        m_rendererAPI->setDepthFunc(function);
    }

    void Renderer::setCullMode(const CullMode mode) {
        m_rendererAPI->setCullMode(mode);
    }

    FrameBuffer& Renderer::getOutputFramebuffer() const {
        return *m_outputFramebuffer;
    }

    FrameBuffer& Renderer::getMainFramebuffer() const {
        return *m_mainFramebuffer;
    }

    void Renderer::setMaterialInShader(Shader& shader, const std::span<Texture* const> textures) {
        KITA_ENGINE_PROFILE("Set material in shader");

        resetTextureState(shader);
        setTexturesInShader(shader, textures);
        shader.setUniformUnsignedInt("textureState", static_cast<uint32_t>(m_textureFlags));
    }

    void Renderer::resetTextureState(Shader& shader) {
        KITA_ENGINE_PROFILE("Reset texture state");

        // depthArray | normal | skybox | stencil | depth | color | cubemap | metallic roughness | albedo
        m_textureFlags = Texture::TextureFlags::NONE;
        shader.setUniformUnsignedInt("textureState", static_cast<uint32_t>(m_textureFlags));
    }

    void Renderer::setTexturesInShader(Shader& shader, const std::span<Texture* const> textures) {
        KITA_ENGINE_PROFILE("Set textures in shader");

        shader.setTextureUniforms();
        for (const auto texture : textures) {
            if (texture == nullptr) {
                continue;
            }
            switch (texture->getType()) {
                case Texture::TextureType::ALBEDO:
                    texture->bind(0);
                    m_textureFlags |= Texture::TextureFlags::ALBEDO;
                    break;
                case Texture::TextureType::METALLIC_ROUGHNESS:
                    texture->bind(1);
                    m_textureFlags |= Texture::TextureFlags::METALLIC_ROUGHNESS;
                    break;
                case Texture::TextureType::CUBEMAP:
                    texture->bind(2);
                    m_textureFlags |= Texture::TextureFlags::CUBEMAP;
                    break;
                case Texture::TextureType::COLOR:
                    texture->bind(3);
                    m_textureFlags |= Texture::TextureFlags::COLOR;
                    break;
                case Texture::TextureType::DEPTH:
                    texture->bind(4);
                    m_textureFlags |= Texture::TextureFlags::DEPTH;
                    break;
                case Texture::TextureType::STENCIL:
                    texture->bind(5);
                    m_textureFlags |= Texture::TextureFlags::STENCIL;
                    break;
                case Texture::TextureType::SKYBOX:
                    texture->bind(6);
                    m_textureFlags |= Texture::TextureFlags::SKYBOX;
                    break;
                case Texture::TextureType::NORMAL:
                    texture->bind(7);
                    m_textureFlags |= Texture::TextureFlags::NORMAL;
                    break;
                case Texture::TextureType::DEPTH_ARRAY:
                    texture->bind(8);
                    m_textureFlags |= Texture::TextureFlags::DEPTH_ARRAY;
                    break;
                default:
                    std::string str = texture->getPath().has_value() ? texture->getPath().value().string() : std::string("missing path");
                    KITA_ENGINE_WARN("Texture: {}, type is NONE or Unknown, unable to bind into shader", str);
                    break;
            }
        }
    }
} // Kita
