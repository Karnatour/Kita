#include "../../../../kitapch.h"
#include "GLFrameBuffer.h"

#include <magic_enum/magic_enum.hpp>

#include "../../../../Core/Engine.h"
#include "../../../../Window/Window.h"

namespace Kita {
    GLFrameBuffer::~GLFrameBuffer() {
        glDeleteFramebuffers(1, &m_fbo);
    }

    void GLFrameBuffer::createBuffer(const std::pair<int, int>& resolution, const std::initializer_list<AttachmentSpec> attachments, const bool highPrecision) {
        m_resolution = resolution;
        m_attachments = attachments;
        m_highPrecision = highPrecision;

        glCreateFramebuffers(1, &m_fbo);

        createAndAttachAttachments();

        if (m_colorTexture == nullptr && m_colorRenderBuffer == nullptr) {
            glNamedFramebufferDrawBuffer(m_fbo, GL_NONE);
            glNamedFramebufferReadBuffer(m_fbo, GL_NONE);
        }

        verifyBuild();
        unbind();
    }

    void GLFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    void GLFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::attachCubemapFace(const unsigned cubemapTexture, const int faceIndex) {
        glNamedFramebufferTextureLayer(m_fbo, GL_COLOR_ATTACHMENT0, cubemapTexture, 0, faceIndex);
    }

    void GLFrameBuffer::resize(const std::pair<int, int>& newResolution) {
        m_resolution = newResolution;
        destroyAttachments();
        createAndAttachAttachments();

        verifyBuild();
        unbind();
    }

    void GLFrameBuffer::createAndAttachAttachments() {
        for (const auto& [bufferType, attachmentType] : m_attachments) {
            switch (attachmentType) {
                case AttachmentType::RENDERBUFFER:
                    createAndAttachRenderBuffer(bufferType);
                    break;
                case AttachmentType::TEXTURE:
                    createAndAttachTexture(bufferType, m_highPrecision);
                    break;
            }
        }
    }

    void GLFrameBuffer::createAndAttachRenderBuffer(const BufferType bufferType) {
        switch (bufferType) {
            case BufferType::COLOR:
                m_colorRenderBuffer = RenderBuffer::createPtr();
                m_colorRenderBuffer->createBuffer(m_resolution, bufferType);
                glNamedFramebufferRenderbuffer(m_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBuffer->getRBO());
                break;
            case BufferType::DEPTH:
                m_depthRenderBuffer = RenderBuffer::createPtr();
                m_depthRenderBuffer->createBuffer(m_resolution, bufferType);
                glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer->getRBO());
                break;
            case BufferType::DEPTH_STENCIL:
                m_depthRenderBuffer = RenderBuffer::createPtr();
                m_depthRenderBuffer->createBuffer(m_resolution, bufferType);
                glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer->getRBO());
                break;
            default:
                KITA_ENGINE_ERROR("Trying to create and attach unsupported buffertype framebuffer renderbuffer {}",magic_enum::enum_name(bufferType));
                break;
        }
    }

    void GLFrameBuffer::createAndAttachTexture(const BufferType bufferType, const bool highPrecision) {
        switch (bufferType) {
            case BufferType::COLOR:
                m_colorTexture = Texture::createPtr();
                m_colorTexture->createBufferTypeTexture(m_resolution, bufferType, highPrecision);
                glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_colorTexture->getTexture(), 0);
                break;
            case BufferType::DEPTH:
                m_depthTexture = Texture::createPtr();
                m_depthTexture->createBufferTypeTexture(m_resolution, bufferType, highPrecision);
                glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, m_depthTexture->getTexture(), 0);
                break;
            case BufferType::DEPTH_STENCIL:
                m_depthTexture = Texture::createPtr();
                m_depthTexture->createBufferTypeTexture(m_resolution, bufferType, highPrecision);
                glNamedFramebufferTexture(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, m_depthTexture->getTexture(), 0);
                break;
            default:
                KITA_ENGINE_ERROR("Trying to create and attach unsupported buffertype framebuffer texture {}",magic_enum::enum_name(bufferType));
                break;
        }
    }

    void GLFrameBuffer::destroyAttachments() {
        for (const auto& [bufferType, attachmentType] : m_attachments) {
            switch (attachmentType) {
                case AttachmentType::RENDERBUFFER:
                    bufferType == BufferType::COLOR ? m_colorRenderBuffer->destroy() : m_depthRenderBuffer->destroy();
                    break;
                case AttachmentType::TEXTURE:
                    bufferType == BufferType::COLOR ? m_colorTexture->destroy() : m_depthTexture->destroy();
                    break;
            }
        }
    }

    void GLFrameBuffer::verifyBuild() {
        if (glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            KITA_ENGINE_ERROR("Framebuffer build error {}", m_fbo);
        }
    }
} // Kita
