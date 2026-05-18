#pragma once
#include "../../RendererAPI.h"
#include <glad/glad.h>

namespace Kita {
    class GLRendererAPI final : public RendererAPI {
    public:
        static GLenum convertBufferTypeToInternalFormat(BufferType buffer);
        static GLenum convertCapablityToGL(Capability capability);
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

        void drawArrays(size_t verticesCount) override;
        void drawElements(size_t indicesCount) override;

        static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
    };
} // Kita
