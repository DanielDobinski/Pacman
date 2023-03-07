cmake_minimum_required(VERSION 3.5)

## vcpkg
SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

## project
project (
	ImGui-GLFW-OpenGL3
	DESCRIPTION "ImGui Sample with GLFW + OpenGL3"
	LANGUAGES CXX)

## Find dependencies
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)

# Use  cmake -S . -B build/ -DOpenGL_GL_PREFERENCE=GLVND
# to skip legacy OpenGL library
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(imgui REQUIRED)


## Create main executable
add_executable(ImGui-GLFW-OpenGL3)

target_include_directories(
	ImGui-GLFW-OpenGL3
  	PRIVATE 
	${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/implot
        ${CMAKE_SOURCE_DIR}/imgui-knobs
        ${CMAKE_SOURCE_DIR}/imspinner
)

IF (WIN32)
     target_link_libraries(ImGui-GLFW-OpenGL3 PRIVATE glfw glad::glad opengl32.lib glm::glm imgui::imgui)
ELSE()
	target_link_libraries(ImGui-GLFW-OpenGL3 PRIVATE glfw glad::glad glm::glm OpenGL imgui::imgui)
ENDIF()


target_sources(
	ImGui-GLFW-OpenGL3
	PRIVATE
        ${CMAKE_SOURCE_DIR}/source/rendering/main.cpp
	${CMAKE_SOURCE_DIR}/source/rendering/resource_manager.cpp
 	${CMAKE_SOURCE_DIR}/source/rendering/texture.cpp
  	${CMAKE_SOURCE_DIR}/source/rendering/sprite_renderer.cpp
  	${CMAKE_SOURCE_DIR}/source/rendering/game.cpp
  	${CMAKE_SOURCE_DIR}/source/rendering/shader_s.cpp
	${CMAKE_SOURCE_DIR}/source/rendering/game_object.cpp
	${CMAKE_SOURCE_DIR}/source/rendering/game_level.cpp
)

