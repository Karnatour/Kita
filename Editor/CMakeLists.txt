cmake_minimum_required(VERSION 4.0)
project(Editor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Editor
        src/Editor.cpp
        src/Editor.h
        src/main.cpp
)

target_compile_options(Editor PRIVATE /MP)

target_link_libraries(Editor PRIVATE KitaEngine Sandbox)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/Sandbox/src)
add_dependencies(Editor KitaEngine Sandbox)