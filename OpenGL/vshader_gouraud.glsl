#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 MVP;
uniform mat4 ModelView;


uniform vec4 LightPosition;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;

uniform float renderingMode;

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

	vec3 tnorm = vec3(ModelView*vec4(VertexNormal,0.0));  /* Como não estou fazendo operacoes com escala/cisalhamento não é necessario usar a matriz de transformacoes normais */
	vec4 model = ModelView*vec4(VertexPosition,1.0f); /* Com isso a cena não é influenciada pela movimentação da camera, mas caso haja movimento dos proprios objetos, isso não será considerado */
	vec3 direction = normalize(vec3(ModelView*LightPosition-model));
	vec3 camera = normalize(-model.xyz);
	vec3 reflected = reflect(-direction,tnorm);
	float light_object_ang = max(dot(direction,tnorm),0.0);
	float camera_reflected_ang = max(dot(reflected,camera), 0.0);

	/* Luz difusa */
	diffuse = Ld * Kd * light_object_ang;

	/* Luz ambiente */
	ambient = La*Ka;

	/* Luz especular */
	specular = vec3(0.0, 0.0, 0.0);
	if(light_object_ang > 0.0)
		specular = clamp(Ls * Ks * pow(camera_reflected_ang, Shininess),0,1);

	
	if(renderingMode == 1) Color = diffuse;
	else if(renderingMode == 2) Color = ambient;
	else if(renderingMode == 3) Color = specular;
	else Color = diffuse + ambient + specular;
	gl_Position = MVP*vec4(VertexPosition[0],VertexPosition[1],VertexPosition[2],1.0);
}
