cmake_minimum_required(VERSION 3.10)
# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define project name
project(hello_animation)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()
# OpenGL
find_package(OpenGL REQUIRED)

# Declare the executable target built from your sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(project_sources
src/array.cc
src/buffer.cc
src/camera.cc
src/scene.cc
src/glad.cc
src/main.cc
src/mesh.cc
src/model.cc
src/program.cc
src/shader.cc
src/stbimg_wrapper.cc
src/timer.cc
src/texture_manager.cc
)
set(project_headers
include/array.h
include/buffer.h
include/camera.h
include/scene.h
include/err.h
include/mesh.h
include/model.h
include/program.h
include/shader.h
include/stbimg_wrapper.h
include/typedef.h
include/glad/glad.h
include/KHR/khrplatform.h
include/stb_image.h
include/timer.h
include/input.h
include/util.h
include/texture_manager.h
)

add_executable(hello_animation 
	${project_sources} 
    ${project_headers}
)
target_include_directories(hello_animation
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(hello_animation
    glfw3
    GL
    GLU 
    X11
    Xrandr
    pthread
    Xi
    dl
    assimp)
set(CMAKE_INCLUDE_CURRENT_DIR ON)