#version 430

layout (location = 0) in vec3 VertexPosition;

layout (location = 0) out vec3 Color;


uniform mat4 MVP;


void main() 
{
	Color = vec3(1.0,0.0,0.0);
	gl_Position = MVP*vec4(VertexPosition[0],VertexPosition[1],VertexPosition[2],1.0);
}