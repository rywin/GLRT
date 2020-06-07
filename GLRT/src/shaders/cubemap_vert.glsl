#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 aTexCoord;

out vec3 cubeCoord;

uniform mat4 view_proj;

void main()
{
	gl_Position = view_proj * vec4(aPos, 1.0);
	cubeCoord = aPos;
}