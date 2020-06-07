#version 430 core

out vec4 FragColor;

in vec3 cubeCoord;

uniform samplerCube cubemap;

void main()
{
	FragColor = texture(cubemap, cubeCoord);
}