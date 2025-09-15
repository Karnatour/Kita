<img src="Engine/assets/textures/logo.png" alt="logo" width="256"/>

*A 3D game engine built with CMake and written in C++ 20*

---

## Used libraries

- **spdlog** (Logging)
- **GLFW** (Window, input management)
- **glad** (OpenGL, Vulkan loader)
- **glm** (Math)
- **stb** (Image loading)
- **assimp** (Asset importer)
- **magic enum** (Used for printing enum values)

---

## Features

- **DLL-based engine**
- **Keyboard & mouse input**
- **3D API independent Renderer** with modern OpenGL 4.6.0 (SSBO, direct state access)  
  *(Vulkan backend planned next)*
- **Buffers:** VBO, IBO, FBO, RBO, UBO, SSBO
- **Blinn–Phong lighting model**
- **HDRI skybox support**
- **Event system (allowing custom events)**
- **Directional shadow mapping** *(point shadows and cascaded shadow mapping wip)*

---

## Building

Tested with **CMake 3.20** and **MSVC 19.44.35207.1** in CLion 2025 and Visual Studio 2022

- Download required libraries with vcpkg
- Add vcpkg toolchain to CMake profile settings
- Build

---

## Usage
Example project can be found in `Sandbox` folder.

User-loaded assets must be placed in the correct folders (relative path then can be used see Sandbox example):

- `assets/textures/` → Textures, skyboxes
- `assets/shaders/` → GLSL shaders
- `assets/models/` → 3D models

When a model is loaded for the first time, **Kita** will:

1. Bake it into its internal **KAsset** format (`.kasset`) (Skipping Assimp loading next time)
2. Copy the model textures into `assets/textures/`