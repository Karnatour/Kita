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
        void render(const std::shared_ptr<Entity>& entity) override;
        void clearColor(float red, float green, float blue, float alpha) override;
        void enableDepthTest() override;
        std::pair<int, int> getViewport() const;

    private:
        void enableTextureInShader(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture);
        void setMaterial(int materialIndex, const std::vector<std::shared_ptr<Material>>& materials, const Transformation& transformation);
        static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
        std::pair<int, int> m_viewport = std::make_pair(1600,900);
    };
} // Kita
