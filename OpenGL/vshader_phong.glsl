#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 ModelView;


uniform vec4 LightPosition;


out vec4 modelPosition;
out vec3 normal;
out vec3 direction;
out vec3 camera;
out vec3 reflected;

/* Phong --> Interpolacao de cores em cada fragmento  -- modelo de reflexao phong */
void main() 
{
	normal = vec3(ModelView*vec4(VertexNormal,0.0));  
	modelPosition = ModelView*vec4(VertexPosition,1.0f); 
	direction = vec3(ModelView*LightPosition-modelPosition);
	camera = -modelPosition.xyz;
	reflected = reflect(-direction,normal);
	//float camera_reflected_ang = max(dot(reflected,camera), 0.0);
	gl_Position = MVP*vec4(VertexPosition[0],VertexPosition[1],VertexPosition[2],1.0);
}
