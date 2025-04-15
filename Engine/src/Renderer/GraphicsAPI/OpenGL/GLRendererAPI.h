#pragma once
#include "../../RendererAPI.h"
#include <glad/glad.h>

namespace Kita {

class GLRendererAPI final : public RendererAPI{
public:
    void setupDebug() override;
    void setViewport(int width, int height) override;
    void render(Mesh& mesh) override;
    void clearColor(float red, float green, float blue, float alpha) override;
private:
    static void debugCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
};

} // Kita
