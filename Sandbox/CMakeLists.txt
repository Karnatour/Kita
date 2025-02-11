cmake_minimum_required(VERSION 3.10)
project(Sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Sandbox src/main.cpp)

target_link_libraries(Sandbox KitaEngine)

target_include_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/Engine/include)

add_dependencies(Sandbox KitaEngine)

