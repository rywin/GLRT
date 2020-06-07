cmake_minimum_required(VERSION 3.2.2)


add_executable(GLRT WIN32  
    src/main.cpp
    src/shaders/quad_vertex.glsl
    src/shaders/quad_fragment.glsl
    src/shaders/compute.glsl
)

set_property(TARGET GLRT PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:GLRT>)

add_custom_command(
         TARGET GLRT POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/GLRT/src/shaders $<TARGET_FILE_DIR:GLRT>/shaders
         COMMENT "Copying shaders" VERBATIM
)
add_custom_command(
         TARGET GLRT POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/GLRT/assets $<TARGET_FILE_DIR:GLRT>/assets
         COMMENT "Copying assets" VERBATIM
)
add_custom_command(
         TARGET GLRT POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy              
             ${CMAKE_SOURCE_DIR}/GLRT/config/imgui.ini $<TARGET_FILE_DIR:GLRT>/
         COMMENT "Copying config files" VERBATIM
)


SOURCE_GROUP(shaders REGULAR_EXPRESSION "./shaders/*$" )

target_include_directories(GLRT PRIVATE ../GL_Core/src ../GL_Core/vendor/imgui/imgui ../GL_Core/vendor/imgui/imgui/examples )

target_link_libraries( GLRT GL_Core glad imgui )
