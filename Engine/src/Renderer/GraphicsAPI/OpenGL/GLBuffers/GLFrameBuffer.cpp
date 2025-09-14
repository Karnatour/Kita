#include "../../../../kitapch.h"
#include "GLFrameBuffer.h"

#include "../../../../Core/Engine.h"
#include "../../../../Window/Window.h"

namespace Kita {
    GLFrameBuffer::~GLFrameBuffer() {
        glDeleteFramebuffers(1, &m_fbo);
    }


    void GLFrameBuffer::createBuffer(const std::pair<int, int>& resolution, const std::initializer_list<AttachmentSpec> attachments) {
        m_resolution = resolution;
        glCreateFramebuffers(1, &m_fbo);

        for (const auto& [bufferType, attachmentType] : attachments) {
            switch (attachmentType) {
                case AttachmentType::RENDERBUFFER:
                    createAndAttachRenderBuffer(bufferType);
                    break;
                case AttachmentType::TEXTURE:
                    createAndAttachTexture(bufferType);
                    break;
            }
        }

        if (m_colorTexture == nullptr && m_colorRenderBuffer == nullptr) {
            glNamedFramebufferDrawBuffer(m_fbo, GL_NONE);
            glNamedFramebufferReadBuffer(m_fbo, GL_NONE);
        }

        if (glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            KITA_ENGINE_ERROR("Framebuffer build error {}", m_fbo);
        }
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

    void GLFrameBuffer::createAndAttachRenderBuffer(BufferType bufferType) {
        switch (bufferType) {
            case BufferType::COLOR:
                m_colorRenderBuffer = RenderBuffer::createPtr();
                m_colorRenderBuffer->createBuffer(m_resolution, BufferType::COLOR);
                glNamedFramebufferRenderbuffer(m_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBuffer->getRBO());
                break;
            case BufferType::DEPTH:
                m_depthRenderBuffer = RenderBuffer::createPtr();
                m_depthRenderBuffer->createBuffer(m_resolution, BufferType::DEPTH);
                glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer->getRBO());
                break;
            case BufferType::STENCIL:
                m_stencilRenderBuffer = RenderBuffer::createPtr();
                m_depthRenderBuffer->createBuffer(m_resolution, BufferType::STENCIL);
                glNamedFramebufferRenderbuffer(m_fbo, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilRenderBuffer->getRBO());
                break;
        }
    }

    void GLFrameBuffer::createAndAttachTexture(BufferType bufferType) {
        switch (bufferType) {
            case BufferType::COLOR:
                m_colorTexture = Texture::createPtr();
                m_colorTexture->createBufferTypeTexture(bufferType, m_resolution);
                glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_colorTexture->getTexture(), 0);
                break;
            case BufferType::DEPTH:
                m_depthTexture = Texture::createPtr();
                m_depthTexture->createBufferTypeTexture(bufferType, m_resolution);
                glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, m_depthTexture->getTexture(), 0);
                break;
            case BufferType::STENCIL:
                m_stencilTexture = Texture::createPtr();
                m_stencilTexture->createBufferTypeTexture(bufferType, m_resolution);
                glNamedFramebufferTexture(m_fbo, GL_STENCIL_ATTACHMENT, m_stencilTexture->getTexture(), 0);
                break;
        }
    }
} // Kita
