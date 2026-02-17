cmake_minimum_required(VERSION 3.20)
project(KitaEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Create DLL
add_library(KitaEngine SHARED
        src/Core/Logger.cpp
        src/Core/Logger.h
        src/Core/DllTemplate.h
        src/Core/Engine.cpp
        src/Core/Engine.h
        include/KitaEngine/Kita.h
        src/Core/DllMain.cpp
        src/Instances/IGameInstance.h
        src/Instances/IGameInstance.cpp
        src/Window/Window.cpp
        src/Window/Window.h
        src/Events/EventManager.h
        src/Events/Event.h
        src/Events/WindowEvents.cpp
        src/Events/WindowEvents.h
        src/Events/MouseEvents.cpp
        src/Events/MouseEvents.h
        src/Events/KeyboardEvents.cpp
        src/Events/KeyboardEvents.h
        src/Input/InputKeys.cpp
        src/Input/InputKeys.h
        src/Events/EventManager.cpp
        src/Input/Input.cpp
        src/Input/Input.h
        src/Window/Callbacks/WindowCallbacks.cpp
        src/Window/Callbacks/WindowCallbacks.h
        src/Window/Callbacks/KeyboardCallbacks.cpp
        src/Window/Callbacks/KeyboardCallbacks.h
        src/Window/Callbacks/MouseCallbacks.cpp
        src/Window/Callbacks/MouseCallbacks.h
        src/Renderer/Buffers/VertexBuffer.cpp
        src/Renderer/Buffers/VertexBuffer.h
        src/Renderer/Buffers/VertexArray.cpp
        src/Renderer/Buffers/VertexArray.h
        src/Renderer/Buffers/IndexBuffer.cpp
        src/Renderer/Buffers/IndexBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLVertexBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLVertexBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLIndexBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLIndexBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLVertexArray.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLVertexArray.h
        src/Renderer/Structs/Vertex.h
        src/Renderer/Renderer.cpp
        src/Renderer/Renderer.h
        src/Renderer/Scene/Primitives/Mesh.cpp
        src/Renderer/Scene/Primitives/Mesh.h
        src/Renderer/Scene/Scene.cpp
        src/Renderer/Scene/Scene.h
        src/Renderer/Structs/RenderingAPI.h
        src/Renderer/Shaders/ShaderManager.cpp
        src/Renderer/Shaders/ShaderManager.h
        src/Renderer/Shaders/Shader.cpp
        src/Renderer/Shaders/Shader.h
        src/Renderer/GraphicsAPI/OpenGL/GLShaders/GLShader.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLShaders/GLShader.h
        src/File/FileReader.cpp
        src/File/FileReader.h
        src/Renderer/RendererAPI.h
        src/Renderer/RendererAPI.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLRendererAPI.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLRendererAPI.h
        src/Renderer/Textures/Texture.cpp
        src/Renderer/Textures/Texture.h
        src/Renderer/GraphicsAPI/OpenGL/GLTextures/GLTexture.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLTextures/GLTexture.h
        src/Renderer/Textures/TextureManager.cpp
        src/Renderer/Textures/TextureManager.h
        vendor/stb_image/stb_image_init.cpp
        src/Renderer/Scene/Camera.cpp
        src/Renderer/Scene/Camera.h
        src/Core/Time.cpp
        src/Core/Time.h
        src/Renderer/Buffers/UniformBuffer.cpp
        src/Renderer/Buffers/UniformBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLUniformBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLUniformBuffer.h
        src/Renderer/Structs/GeometryHelper.h
        src/Asset/KAsset.cpp
        src/Asset/KAsset.h
        src/Renderer/Scene/Primitives/Material.cpp
        src/Renderer/Scene/Primitives/Material.h
        src/Renderer/Scene/Primitives/Model.cpp
        src/Renderer/Scene/Primitives/Model.h
        src/Renderer/Structs/PhongProperties.h
        src/Asset/AssetImporter.cpp
        src/Asset/AssetImporter.h
        src/Renderer/Structs/GeometryHelper.cpp
        src/Renderer/Scene/Entity.cpp
        src/Renderer/Scene/Entity.h
        src/Renderer/Scene/Transformation.cpp
        src/Renderer/Scene/Transformation.h
        src/Renderer/Scene/Entities/LightEntity.cpp
        src/Renderer/Scene/Entities/LightEntity.h
        src/Renderer/Buffers/ShaderStorageBuffer.cpp
        src/Renderer/Buffers/ShaderStorageBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLShaderStorageBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLShaderStorageBuffer.h
        src/Renderer/Scene/Entities/SkyboxEntity.cpp
        src/Renderer/Scene/Entities/SkyboxEntity.h
        src/Renderer/Buffers/FrameBuffer.cpp
        src/Renderer/Buffers/FrameBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLFrameBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLFrameBuffer.h
        src/Renderer/Buffers/RenderBuffer.cpp
        src/Renderer/Buffers/RenderBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLRenderBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLBuffers/GLRenderBuffer.h
        src/Renderer/Structs/RendererTypes.h
)

#Multithreaded compilation
target_compile_options(KitaEngine PRIVATE /MP)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC spdlog::spdlog)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glad::glad)

find_package(glm CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glm::glm)

find_package(Stb REQUIRED)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC assimp::assimp)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(KitaEngine PRIVATE magic_enum::magic_enum)

get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Imported targets: ${_targets}")

target_include_directories(KitaEngine PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_precompile_headers(KitaEngine PRIVATE ${PROJECT_SOURCE_DIR}/src/kitapch.h)

#Add compile definitions for DLL
target_compile_definitions(KitaEngine PRIVATE KITAENGINE_EXPORTS KITA_BUILD_DEBUG)