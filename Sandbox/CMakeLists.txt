cmake_minimum_required(VERSION 3.20)
project(Sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Create Sandbox executable
add_executable(Sandbox src/main.cpp)

#Include KitaEngine DLL
target_link_libraries(Sandbox PUBLIC KitaEngine)
target_include_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(Sandbox KitaEngine)

