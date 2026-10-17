#pragma once
#include "../../RendererAPI.h"
#include <glad/glad.h>

namespace Kita {
    class GLRendererAPI final : public RendererAPI {
    public:
        static GLenum convertBufferTypeToInternalFormat(BufferType buffer);
        static GLenum convertCapabilityToGL(Capability capability);
        static GLenum convertDrawModeToGL(DrawMode drawMode);
        static GLbitfield convertBitToGL(ClearBit bit);

    private:
        void setupDebug() override;

        void setViewport(std::pair<int, int> resolution) override;

        void clearColor(float red, float green, float blue, float alpha) override;
        void clearBit(std::span<const ClearBit> bits) override;

        void enableCapability(Capability capability) override;
        void disableCapability(Capability capability) override;
        void enableBufferWrite(BufferType bufferType) override;
        void disableBufferWrite(BufferType bufferType) override;
        void setDepthFunc(DepthFunction function) override;
        void setCullMode(CullMode mode) override;

        void drawArrays(DrawMode mode, size_t verticesCount) override;
        void multiDrawArraysIndirect(DrawMode mode, size_t drawCount) override;

        void drawElements(DrawMode mode, size_t indicesCount) override;
        void multiDrawElementsIndirect(DrawMode mode, size_t drawCount) override;

        static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
    };
} // Kita
