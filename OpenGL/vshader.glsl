#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform vec3 Kd;
uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat3 Normal;
uniform vec4 LightPosition;
uniform vec3 Ld;

layout (location = 0) out vec3 Color;

void main() 
{
	vec3 tnorm = normalize(Normal*VertexNormal);
	vec4 eyeCoords = ModelView*vec4(VertexPosition,1.0f);
	vec3 s = normalize(vec3(LightPosition-eyeCoords));
	vec3 LightIntensity = Ld * Kd * max(dot(s,tnorm),0.0);

	Color = LightIntensity;
	gl_Position = MVP*vec4(VertexPosition[0],VertexPosition[1],VertexPosition[2],1.0);
}
