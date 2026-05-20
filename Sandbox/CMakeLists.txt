cmake_minimum_required(VERSION 4.0)
project(Sandbox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Sandbox SHARED
        src/Sandbox.cpp
        src/Sandbox.h
)

# Multithreaded compilation
target_compile_options(Sandbox PRIVATE /MP)

# Add compile definitions for DLL
target_compile_definitions(Sandbox PRIVATE SANDBOX_BUILD_DLL_EXPORTS)

target_link_libraries(Sandbox PUBLIC KitaEngine)
add_dependencies(Sandbox KitaEngine)