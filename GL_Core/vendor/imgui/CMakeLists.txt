cmake_minimum_required(VERSION 3.2.2)

set(IMGUI_INCLUDE_DIRS imgui)


add_library( imgui STATIC
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imgui_demo.cpp
    imgui/imgui_demo.cpp
)

target_include_directories(imgui PRIVATE imgui)
target_link_libraries( imgui glfw glad ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
