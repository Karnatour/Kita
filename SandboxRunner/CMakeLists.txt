cmake_minimum_required(VERSION 4.2)
project(SandboxRunner)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SandboxRunner src/main.cpp)

target_compile_options(SandboxRunner PRIVATE /MP)

target_link_libraries(SandboxRunner PRIVATE KitaEngine Sandbox)
target_include_directories(SandboxRunner PRIVATE ${CMAKE_SOURCE_DIR}/Sandbox/src)
add_dependencies(SandboxRunner KitaEngine Sandbox)

add_dependencies(SandboxRunner CopyAssets)