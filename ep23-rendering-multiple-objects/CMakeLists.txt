cmake_minimum_required(VERSION 3.3)
project(ep17-textures)
if (CMAKE_SYSTEM MATCHES "Linux")
    add_definitions(-D__Linux__)
endif()


find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)
set(MESA_GL_VERSION_OVERRIDE "310")
set(MESA_GLSL_VERSION_OVERRIDE "310")
add_executable(ep17-textures Application.cpp  VertexArray.cpp VertexBuffer.cpp IndexBuffer.cpp Shader.cpp Texture.cpp Renderer.cpp vendor/stb_image/stb_image.cpp imgui_impl_glfw_gl3.h
        vendor/imgui/imconfig.h vendor/imgui/imgui.cpp vendor/imgui/imgui.h vendor/imgui/imgui-main.cpp_example
        vendor/imgui/imgui_demo.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_impl_glfw_gl3.cpp vendor/imgui/imgui_impl_glfw_gl3.h
        vendor/imgui/imgui_internal.h vendor/imgui/stb_rect_pack.h vendor/imgui/stb_textedit.h vendor/imgui/stb_truetype.h)
target_link_libraries(ep17-textures glfw GLEW libGLEW.so libGLU.so libGL.so)

