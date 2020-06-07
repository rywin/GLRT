cmake_minimum_required(VERSION 3.2.2)
project(GLRT)

find_package(OpenGL REQUIRED)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/GL_Core/vendor)

set(GLFW3_DIR ${VENDOR_DIR}/glfw)
set(GLAD_DIR ${VENDOR_DIR}/glad)
set(GLM_DIR ${VENDOR_DIR}/glm)
set(IMGUI_DIR ${VENDOR_DIR}/imgui)


add_subdirectory(${GLFW3_DIR})

if( WIN32 )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" )
endif()

if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set (PROJECTNAME_ARCHITECTURE "x64")
else ()
    set (PROJECTNAME_ARCHITECTURE "x86")
endif ()

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLRT' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLRT )
    endif()
endif()

add_subdirectory(${GLAD_DIR})

add_subdirectory(${GLM_DIR})

add_subdirectory(${IMGUI_DIR})

add_subdirectory(GL_Core)
add_subdirectory(GLRT)