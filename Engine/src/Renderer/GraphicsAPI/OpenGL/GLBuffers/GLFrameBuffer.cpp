#include "../../../../kitapch.h"
#include "GLFrameBuffer.h"

#include <glad/glad.h>

#include "../../../../Core/Engine.h"
#include "../../../../Window/Window.h"

namespace Kita {
    GLFrameBuffer::~GLFrameBuffer() {
        glDeleteFramebuffers(1, &m_fbo);
    }

    void GLFrameBuffer::createBuffer(const std::pair<int, int>& resolution, const unsigned int colorAsRenderBuffer, const unsigned int depthAsRenderBuffer, const unsigned int stencilAsRenderBuffer) {
        m_resolution = resolution;
        glCreateFramebuffers(1, &m_fbo);

        if (colorAsRenderBuffer != 0) {
            glNamedFramebufferRenderbuffer(m_fbo,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER, colorAsRenderBuffer);
        }
        else {
            createAndAttachTextureToFBO(m_colorTexture, BufferType::COLOR);
        }

        if (depthAsRenderBuffer != 0) {
            glNamedFramebufferRenderbuffer(m_fbo,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, colorAsRenderBuffer);
        }
        else {
            createAndAttachTextureToFBO(m_depthTexture, BufferType::DEPTH);
        }

        if (stencilAsRenderBuffer != 0) {
            glNamedFramebufferRenderbuffer(m_fbo,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER, colorAsRenderBuffer);
        }
        else {
            createAndAttachTextureToFBO(m_stencilTexture, BufferType::STENCIL);
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

    void GLFrameBuffer::createAndAttachTextureToFBO(unsigned int& texture, const BufferType& type) {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        switch (type) {
            case BufferType::COLOR:
                glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureStorage2D(texture, 1, GL_RGB8, m_resolution.first, m_resolution.second);
                glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, texture, 0);
                break;
            case BufferType::DEPTH:
                glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(texture, 1, GL_DEPTH_COMPONENT24, m_resolution.first, m_resolution.second);
                glNamedFramebufferTexture(m_fbo, GL_DEPTH_COMPONENT24, m_colorTexture, 0);
                break;
            case BufferType::STENCIL:
                glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(texture, 1, GL_STENCIL_INDEX8, m_resolution.first, m_resolution.second);
                glNamedFramebufferTexture(m_fbo, GL_STENCIL_ATTACHMENT, texture, 0);
                break;
        }
    }
} // Kita
