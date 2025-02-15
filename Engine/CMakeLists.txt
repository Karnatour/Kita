cmake_minimum_required(VERSION 3.10)
project(KitaEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)



#Create DLL
add_library(KitaEngine SHARED
        src/Logger.cpp
        src/Logger.h
        src/DLLTemplate.h
        src/Engine.cpp
        src/Engine.h
        include/KitaEngine/Kita.h
        src/DllMain.cpp
        src/IGameInstance.h
        src/IGameInstance.cpp
        src/Window.cpp
        src/Window.h)



find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC spdlog::spdlog)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(KitaEngine PUBLIC glad::glad)

get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Imported targets: ${_targets}")

target_include_directories(KitaEngine PUBLIC ${CMAKE_SOURCE_DIR}/Engine/include)

#Add compile definitions for DLL
target_compile_definitions(KitaEngine PRIVATE KITAENGINE_EXPORTS KITA_BUILD_DEBUG)
