cmake_minimum_required(VERSION 4.0)
project(SandboxRunner)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SandboxRunner src/main.cpp)

target_compile_options(SandboxRunner PRIVATE /MP)

target_link_libraries(SandboxRunner PRIVATE KitaEngine Sandbox)
target_include_directories(SandboxRunner PRIVATE ${CMAKE_SOURCE_DIR}/Sandbox/src)
add_dependencies(SandboxRunner KitaEngine Sandbox)

# Asset copying
file(GLOB_RECURSE ASSET_FILES
        RELATIVE ${CMAKE_SOURCE_DIR}/Engine/assets
        ${CMAKE_SOURCE_DIR}/Engine/assets/*)

set(ASSET_OUTPUT_FILES "")
set(ASSETS_SRC ${CMAKE_SOURCE_DIR}/Engine/assets)

foreach (ASSET ${ASSET_FILES})
    set(SRC ${ASSETS_SRC}/${ASSET})
    set(STAMP ${CMAKE_BINARY_DIR}/asset_stamps/${ASSET}.stamp)

    get_filename_component(ASSET_SUBDIR ${ASSET} DIRECTORY)
    get_filename_component(STAMP_DIR ${STAMP} DIRECTORY)

    file(MAKE_DIRECTORY ${STAMP_DIR})
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Debug/assets/${ASSET_SUBDIR})
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release/assets/${ASSET_SUBDIR})

    add_custom_command(
            OUTPUT ${STAMP}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SRC}
            ${CMAKE_BINARY_DIR}/$<CONFIG>/assets/${ASSET}
            COMMAND ${CMAKE_COMMAND} -E touch ${STAMP}
            DEPENDS ${SRC}
            COMMENT "Copying asset: ${ASSET}"
    )
    list(APPEND ASSET_OUTPUT_FILES ${STAMP})
endforeach ()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/Engine/assets/shaders
        ${CMAKE_SOURCE_DIR}/Engine/assets/textures
        ${CMAKE_SOURCE_DIR}/Engine/assets/models)

add_custom_target(CopyAssets ALL DEPENDS ${ASSET_OUTPUT_FILES})
add_dependencies(SandboxRunner CopyAssets)