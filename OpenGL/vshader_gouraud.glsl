#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat3 Normal;


uniform vec4 LightPosition;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;

/*struct LightInfo {
	vec4 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

uniform LightInfo Light;

struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo Material;*/

layout (location = 0) out vec3 Color;

/* Gouraud --> Interpolacao de cores nos vertices -- modelo de reflexao phong */
void main() 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 tnorm = normalize(Normal*VertexNormal);
	vec4 eyeCoords = ModelView*vec4(VertexPosition,1.0f);
	vec3 s = normalize(vec3(LightPosition-eyeCoords));
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s,tnorm);
	float sDotN = max(dot(s,tnorm),0.0);

	/* Luz difusa */
	diffuse = Ld * Kd * sDotN;

	/* Luz ambiente */
	ambient = La*Ka;

	/* Luz especular */
	specular = vec3(0.0, 0.0, 0.0);
	if(sDotN > 0.0)
		specular = Ls * Ks * pow(max(dot(r,v), 0.0), Shininess);

	Color = min(diffuse + ambient + specular,1.0);
	gl_Position = MVP*vec4(VertexPosition[0],VertexPosition[1],VertexPosition[2],1.0);
}
