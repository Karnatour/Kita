#include "../kitapch.h"
#include "Renderer.h"
#include "../Assets/Shader.h"
#include "../Core/Engine.h"
#include "Scene/Primitives/Mesh.h"
#include "../Events/EventManager.h"

namespace Kita {
    Renderer::Renderer(const RenderingAPI api) {
        m_api = api;
        m_rendererAPI = RendererAPI::createPtr(api);
        m_rendererAPI->init(*this);

        EventManager::listenToEvent<FrameBufferResized>(onFrameBufferResize);
        m_mainFramebuffer->createBuffer(
            m_viewport, std::initializer_list<FrameBuffer::AttachInfo>{{BufferType::COLOR, FrameBuffer::AttachType::TEXTURE}, {BufferType::DEPTH_STENCIL, FrameBuffer::AttachType::RENDERBUFFER}},
            true, 1);
    }

    RenderingAPI Renderer::getAPI() {
        return m_api;
    }

    std::pair<float, float> Renderer::getViewport() const {
        return m_viewport;
    }

    void Renderer::renderMesh(const Mesh& mesh, Shader& shader, const glm::mat4& modelMatrix, const std::span<Texture* const> textures) {
        shader.bind();
        setMaterialInShader(shader, textures, modelMatrix);

        mesh.getVertexArray().bind();
        if (mesh.getIndexBuffer()) {
            m_rendererAPI->drawElements(mesh.getIndexBuffer()->getIndices().size());
        }
        else {
            m_rendererAPI->drawArrays(mesh.getVertexBuffer().getVertices().size());
        }
    }

    void Renderer::onFrameBufferResize(const FrameBufferResized& event) {
        Engine::getEngine()->getRenderer().setViewport(event.getSize(), true);
        Engine::getEngine()->getRenderer().m_mainFramebuffer->resize(event.getSize());
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
        m_rendererAPI->clearColor(red,green,blue,alpha);
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

    FrameBuffer& Renderer::getMainFramebuffer() const {
        return *m_mainFramebuffer;
    }

    void Renderer::setMaterialInShader(Shader& shader, const std::span<Texture* const> textures, const glm::mat4& modelMatrix) {
        resetTextureState(shader);
        setTexturesInShader(shader, textures);
        shader.setUniformMat4("model",modelMatrix);
    }

    void Renderer::resetTextureState(Shader& shader) {
        shader.setUniformBool("hasDiffuseTex", false);
        shader.setUniformBool("hasSpecularTex", false);
        shader.setUniformBool("hasCubemapTex", false);
        shader.setUniformBool("hasColorTex", false);
        shader.setUniformBool("hasDepthTex", false);
        shader.setUniformBool("hasStencilTex", false);
    }

    void Renderer::setTexturesInShader(Shader& shader, const std::span<Texture* const> textures) {
        for (const auto texture : textures) {
            switch (texture->getType()) {
                case Texture::TextureType::DIFFUSE:
                    texture->bind(0);
                    shader.setUniformInt("diffuseTex", 0);
                    shader.setUniformBool("hasDiffuseTex", true);
                    break;
                case Texture::TextureType::SPECULAR:
                    texture->bind(1);
                    shader.setUniformInt("specularTex", 1);
                    shader.setUniformBool("hasSpecularTex", true);
                    break;
                case Texture::TextureType::CUBEMAP:
                    texture->bind(2);
                    shader.setUniformInt("cubemapTex", 2);
                    shader.setUniformBool("hasCubemapTex", true);
                    break;
                case Texture::TextureType::COLOR:
                    texture->bind(3);
                    shader.setUniformInt("colorTex", 3);
                    shader.setUniformBool("hasColorTex", true);
                    break;
                case Texture::TextureType::DEPTH:
                    texture->bind(4);
                    shader.setUniformInt("depthTex", 4);
                    shader.setUniformBool("hasDepthTex", true);
                    break;
                case Texture::TextureType::STENCIL:
                    texture->bind(5);
                    shader.setUniformInt("stencilTex", 5);
                    shader.setUniformBool("hasStencilTex", true);
                    break;
                default:
                    KITA_ENGINE_WARN("Texture {}, type is NONE or Unknown, unable to bind into shader", texture->getPath().string());
                    break;
            }
        }
    }
} // Kita
