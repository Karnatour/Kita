#include "../../../kitapch.h"
#include "GLRendererAPI.h"

#include <glad/glad.h>

namespace Kita {
    void GLRendererAPI::setViewport(const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void GLRendererAPI::render(Mesh& mesh) {
        mesh.getVertexArray()->bind();
        mesh.getShader()->bind();
        const auto& vertexArray = mesh.getVertexArray();
        if (vertexArray->getIBOobj()->getIndicesCount() == 0) {
            glDrawArrays(GL_TRIANGLES,0,static_cast<GLsizei>(vertexArray->getVBOobj()->getVerticiesCount()));
        }
        else {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray->getIBOobj()->getIndicesCount()),GL_UNSIGNED_INT,nullptr);
        }
    }

    void GLRendererAPI::clearColor(const float red,const float green,const float blue, const float alpha) {
        glClearColor(red,green,blue,alpha);
        glClear(GL_COLOR_BUFFER_BIT);
    }
} // Kita
