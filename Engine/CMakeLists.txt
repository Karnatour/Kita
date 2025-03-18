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
        src/Renderer/VertexBuffer.cpp
        src/Renderer/VertexBuffer.h
        src/Renderer/VertexArray.cpp
        src/Renderer/VertexArray.h
        src/Renderer/ElementBuffer.cpp
        src/Renderer/ElementBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLVertexBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLVertexBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLElementBuffer.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLElementBuffer.h
        src/Renderer/GraphicsAPI/OpenGL/GLVertexArray.cpp
        src/Renderer/GraphicsAPI/OpenGL/GLVertexArray.h
        src/Renderer/Structs/Vertex.h
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

target_precompile_headers(KitaEngine PRIVATE src/kitapch.h)

#Add compile definitions for DLL
target_compile_definitions(KitaEngine PRIVATE KITAENGINE_EXPORTS KITA_BUILD_DEBUG)

