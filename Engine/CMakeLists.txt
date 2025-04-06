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
        src/Events/EngineListeners.cpp
        src/Events/EngineListeners.h
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
        src/Renderer/Mesh.cpp
        src/Renderer/Mesh.h
        src/Renderer/Scene.cpp
        src/Renderer/Scene.h
        src/Renderer/Structs/RenderingAPI.h
        src/Renderer/Shaders/ShaderManager.cpp
        src/Renderer/Shaders/ShaderManager.h
        src/Renderer/Shaders/Shader.cpp
        src/Renderer/Shaders/Shader.h
        src/Renderer/GraphicsAPI/OpenGL/GLShaders/GLShader.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLShaders/GLShader.h
        src/Core/FileReader.cpp
        src/Core/FileReader.h
        src/Renderer/RendererAPI.h
        src/Renderer/RendererAPI.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLRendererAPI.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLRendererAPI.h
)



find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC spdlog::spdlog)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glad::glad)

find_package(glm CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glm::glm)

get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Imported targets: ${_targets}")

target_include_directories(KitaEngine PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_precompile_headers(KitaEngine PRIVATE ${PROJECT_SOURCE_DIR}/src/kitapch.h)

#Add compile definitions for DLL
target_compile_definitions(KitaEngine PRIVATE KITAENGINE_EXPORTS KITA_BUILD_DEBUG)

