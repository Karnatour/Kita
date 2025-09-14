#pragma once
#include "../../RendererAPI.h"
#include <glad/glad.h>

namespace Kita {
    class Transformation;
    class Material;
    class Texture;
    class Shader;

    class GLRendererAPI final : public RendererAPI {
    public:
        void setupDebug() override;

        void setViewport(int width, int height, bool rewriteStoredPair) override;
        void restoreViewport() override;

        void clearColor(float red, float green, float blue, float alpha) override;
        void clearBit(std::initializer_list<ClearBit> bits) override;

        void enableCapability(const Capabilities& capability) override;
        void disableCapability(const Capabilities& capability) override;
        void enableBufferWrite(const BufferType& bufferType) override;
        void disableBufferWrite(const BufferType& bufferType) override;
        void setDepthFunc(const DepthFunctions& function) override;

        void drawArrays(const size_t& verticesCount) override;
        void drawElements(const size_t& indicesCount) override;

    private:
        static GLenum convertCapablityToGL(Capabilities capability);
        static GLbitfield convertBitToGL(ClearBit bit);
        static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
    };
} // Kita
