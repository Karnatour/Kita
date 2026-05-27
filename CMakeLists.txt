cmake_minimum_required(VERSION 4.2)
project(Kita)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

# Set the output directory for libraries and executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/bin)

add_subdirectory(Editor)
add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(SandboxRunner)

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
        ${CMAKE_SOURCE_DIR}/Engine/assets/models
        ${CMAKE_SOURCE_DIR}/Engine/assets/fonts
)

add_custom_target(CopyAssets ALL DEPENDS ${ASSET_OUTPUT_FILES})