#include "GeometryUtil.h"

namespace Kita {
    MeshData Geometry::getCubeData() {
        std::vector<std::byte> vertices;
        vertices.reserve(VertexLayout::getDefaultVertexLayout().getStride() * 24);

        // ===== Front face =====
        // Vertex 0
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, 1.0f)); // Position
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Color
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f)); // TextureCoords
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f)); // Normal
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f)); // Tangent
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f)); // Bitangent
        // Vertex 1
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 2
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 3
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

        // ===== Top face =====
        // Vertex 4
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        // Vertex 5
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        // Vertex 6
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        // Vertex 7
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));

        // ===== Back face =====
        // Vertex 8
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 9
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 10
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 11
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

        // ===== Bottom face =====
        // Vertex 12
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        // Vertex 13
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        // Vertex 14
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        // Vertex 15
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));

        // ===== Left face =====
        // Vertex 16
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 17
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 18
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 19
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

        // ===== Right face =====
        // Vertex 20
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 21
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 22
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 23
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

        return MeshData{
            .vertexBufferData = VertexBufferData(std::move(vertices), VertexLayout::getDefaultVertexLayout()),
            .indices = {
                0, 1, 2, 2, 3, 0, // front
                4, 5, 6, 6, 7, 4, // top
                8, 9, 10, 10, 11, 8, // back
                12, 13, 14, 14, 15, 12, // bottom
                16, 17, 18, 18, 19, 16, // left
                20, 21, 22, 22, 23, 20 // right
            }
        };
    }

    MeshData Geometry::getQuadData() {
        std::vector<std::byte> vertices;
        vertices.reserve(VertexLayout::getDefaultVertexLayout().getStride() * 4);

        // Vertex 0
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, 1.0f, 0.0f)); // Position
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Color
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 1.0f)); // TextureCoords
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f)); // Normal
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f)); // Tangent
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f)); // Bitangent
        // Vertex 1
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 2
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 3
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, -1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

        return MeshData{
            .vertexBufferData = VertexBufferData(std::move(vertices), VertexLayout::getDefaultVertexLayout()),
            .indices = {
                0, 1, 2,
                0, 2, 3
            }
        };
    }

    MeshData Geometry::getTriangleData() {
        std::vector<std::byte> vertices;
        vertices.reserve(VertexLayout::getDefaultVertexLayout().getStride() * 3);

        // Vertex 0 (top, red)
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f)); // Position
        VertexBufferData::appendBytes(vertices, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Color
        VertexBufferData::appendBytes(vertices, glm::vec2(0.5f, 1.0f)); // TextureCoords
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f)); // Normal
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f)); // Tangent
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f)); // Bitangent
        // Vertex 1 (bottom left, green)
        VertexBufferData::appendBytes(vertices, glm::vec3(-1.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
        // Vertex 2 (bottom right, blue)
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, -1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec2(1.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 0.0f, 1.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(1.0f, 0.0f, 0.0f));
        VertexBufferData::appendBytes(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

        return MeshData{
            .vertexBufferData = VertexBufferData(std::move(vertices), VertexLayout::getDefaultVertexLayout()),
            .indices = {
                0, 1, 2
            }
        };
    }

    std::string Geometry::getCubeKey() {
        return "Kita::Geometry::Cube";
    }

    std::string Geometry::getQuadKey() {
        return "Kita::Geometry::Quad";
    }

    std::string Geometry::getTriangleKey() {
        return "Kita::Geometry::Triangle";
    }
}
