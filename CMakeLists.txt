cmake_minimum_required(VERSION 3.20)
project(Kita)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

# Set the output directory for libraries and executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/bin)

add_compile_options(/MP) # MultiThreaded compilation

add_subdirectory(Engine)
add_subdirectory(Sandbox)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY ${CMAKE_SOURCE_DIR}/Engine/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(COPY ${CMAKE_SOURCE_DIR}/Engine/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
endif()