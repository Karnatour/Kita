cmake_minimum_required(VERSION 3.10)
project(KitaEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(KitaEngine SHARED src/test.cpp)

target_include_directories(KitaEngine PUBLIC ${CMAKE_SOURCE_DIR}/Engine/include)

target_compile_definitions(KitaEngine PRIVATE KITAENGINE_EXPORTS)
