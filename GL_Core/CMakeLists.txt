cmake_minimum_required(VERSION 3.2.2)

add_library( GL_Core STATIC
    src/Gl_Core.hpp
    src/Camera.cpp
    src/Camera.hpp
    src/Cube.cpp
    src/Cube.hpp
    src/FpsController.cpp
    src/FpsController.hpp
    src/Shader.cpp
    src/Shader.hpp
    src/stb_image.h
    src/stb_image_write.h
    src/Texture.cpp
    src/Texture.hpp
    src/Window.cpp
    src/Window.hpp
	src/Quad.cpp
	src/Quad.hpp
	src/ComputeShader.cpp
	src/ComputeShader.hpp
	src/CubeMap.cpp
	src/CubeMap.hpp
	src/Framebuffer.cpp
	src/Framebuffer.hpp
)

target_link_libraries(GL_Core
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES} 
    glad ${GLAD_LIBRARIES} 
    glm ${GLM_LIBRARIES} 

)