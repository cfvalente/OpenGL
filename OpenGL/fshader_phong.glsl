#version 430

in vec4 modelPosition;
in vec3 normal;
in vec3 direction;
in vec3 reflected;

uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;

uniform float renderingMode;

out vec4 FragColor;


void main ()  
{     
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 n = normalize(normal);
	vec3 s = normalize(direction);
	float light_object_ang = max(dot(s,n),0.0);
	vec3 r = normalize(reflected);
	vec3 camera = vec3(-modelPosition);
	float camera_reflected_ang = max(dot(r,camera), 0.0);

	diffuse = vec4(Ld*Kd*light_object_ang,1.0);

	ambient = vec4(La*Ka,1.0);

	specular = vec4(0.0, 0.0, 0.0, 1.0);
	if(light_object_ang > 0.0)
		specular = clamp(vec4(Ls,1.0) * vec4(Ks,1.0) * pow(camera_reflected_ang, Shininess),0,1);

	if(renderingMode == 1) FragColor = diffuse;
	else if(renderingMode == 2) FragColor = ambient;
	else if(renderingMode == 3) FragColor = specular;
	else FragColor = ambient + diffuse + specular; 
}  