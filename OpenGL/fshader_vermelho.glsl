#version 430

layout (location = 0) in vec3 Color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(Color, 1.0);  
}