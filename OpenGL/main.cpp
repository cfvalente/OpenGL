#include <iostream>
#include <string>

#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"

#include "Include/glm/glm.hpp"
#include "Include/glm/gtc/matrix_transform.hpp"

#include "file.h"
#include "info.h"
#include "loader.h"



#ifdef SPONZA
#define MODEL "Model/sponza.obj"
#else
#ifdef TEAPOT
#define MODEL "SimpleModel/teapot.obj"
#else
#define MODEL "SimpleModel/simples.obj"
#endif
#endif
//#define MODEL "SimpleModel/simples.obj"
// "SimpleModel/teapot.obj"
// "Model/sponza.obj"
using namespace std;
using namespace glm;



GLuint programHandle;
GLuint *vaoHandle;
model model_data;

vec4 *LightPosition;
vec3 *Ld;

mat4 ModelView;
mat4 OriginalModelView;
mat4 Projection;
mat3 Normal;

vec3 translate_value;
vec3 scale_value;
float rotation_value;

GLuint *elementBufferHandle;
GLFWwindow* window;


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		translate_value[0] = translate_value[0]+0.3f;
	if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		translate_value[0] = translate_value[0]-0.3f;
	if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		translate_value[2] = translate_value[2]+0.3f;
	if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		translate_value[2] = translate_value[2]-0.3f;
}



/* Inicializacao de variaveis e objetos */
void init(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1280, 1024, "Shader", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE; 
	glewInit();
	glViewport(0,0,1280,1024);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f);

	OriginalModelView = glm::mat4(1.0);
	Projection = glm::mat4(1.0);
	translate_value = vec3(0.0f,0.0f,-30.0f);
	scale_value = vec3(1.0f,1.0f,1.0f);
	rotation_value = 0.0f;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(45.0f, 1280.0f / 1024.0f, 0.1f, 500.0f);

	// Camera is at (4,3,3), in World Space, and looks at the origin, head is up (set to 0,-1,0 to look upside-down)
	OriginalModelView = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));


	model_data = load_model(MODEL);

	vaoHandle = new GLuint[model_data.num_meshes];
	elementBufferHandle = new GLuint[model_data.num_meshes];
	for(unsigned int m = 0; m<model_data.num_meshes; m++)
	{
		GLuint vboHandles[2];
		glGenBuffers(1,&vboHandles[0]);
		GLuint positionBufferHandle = vboHandles[0];
		glGenBuffers(1,&vboHandles[1]);
		GLuint normalBufferHandle = vboHandles[1];

		glGenBuffers(1, &elementBufferHandle[m]);

		glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
		glBufferData(GL_ARRAY_BUFFER,3*model_data.num_vertices[m]*sizeof(float),model_data.position_data[m],GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
		glBufferData(GL_ARRAY_BUFFER,3*model_data.num_vertices[m]*sizeof(float),model_data.normal_data[m],GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferHandle[m]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * model_data.num_faces[m] * sizeof(GL_UNSIGNED_INT), model_data.face_data[m], GL_STATIC_DRAW);

		glGenVertexArrays(1,&vaoHandle[m]);
		glBindVertexArray(vaoHandle[m]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte *)NULL);

	}

}


/* Cuida da inicializacao da luz */
void light()
{
	LightPosition = new glm::vec4[1];
	Ld = new glm::vec3[1];
	LightPosition[0] = glm::vec4(0.0f,0.0f,20.0f,1.0f);
	Ld[0] = glm::vec3(1.0f,1.0f,1.0f);
}



void movement()
{   
	//translate_value[0]  = translate_value[0] + 0.02f;
	//rotation_value = rotation_value+1.0f;
	ModelView = OriginalModelView;
	//ModelView = rotate(ModelView,rotation_value,vec3(0.0f,1.0f,0.0f));
	ModelView = translate(ModelView,translate_value);
	ModelView =  rotate(ModelView,rotation_value,vec3(1.0f,0.0f,0.0f));
	//ModelView = glm::scale(ModelView,scale);
}

void display()
{
	mat4 MVP;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MVP = Projection * ModelView;
	//ModelView = View*Model; /* Usar ModelView para calcular a normal ou apenas Model? --> Apenas Model, pois a luz nao esta no espaco da camera */
	Normal = transpose(inverse(mat3(ModelView)));

	GLuint location = glGetUniformLocation(programHandle,"MVP");
	glUniformMatrix4fv(location, 1, GL_FALSE, &MVP[0][0]);
	location = glGetUniformLocation(programHandle,"ModelView");
	glUniformMatrix4fv(location, 1, GL_FALSE, &ModelView[0][0]);
	location = glGetUniformLocation(programHandle,"Normal");
	glUniformMatrix3fv(location, 1, GL_FALSE, &Normal[0][0]);
	location = glGetUniformLocation(programHandle,"LightPosition");
	glUniform4f(location,0.0f,0.0f,40.0f,1.0f);
	location = glGetUniformLocation(programHandle,"Ld");
	glUniform3f(location,1.0f,1.0f,1.0f);
	for(unsigned int m = 0; m < model_data.num_meshes; m++)
	{
		glBindVertexArray(vaoHandle[m]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBufferHandle[m]);
		location = glGetUniformLocation(programHandle,"Kd");
		glUniform3f(location,model_data.material_kd[model_data.mesh_material[m]][0],model_data.material_kd[model_data.mesh_material[m]][1],model_data.material_kd[model_data.mesh_material[m]][2]);

		glDrawElements(GL_TRIANGLES, 3 * model_data.num_faces[m] * sizeof(GL_UNSIGNED_INT), GL_UNSIGNED_INT, (void*)0);
	}
	glfwSwapBuffers(window);
}

void set_shader()
{
	GLint size[2];
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	programHandle = glCreateProgram();
	if(vshader == 0) cout << "Error: Vertex Shader\n";
	if(fshader == 0) cout << "Error: Fragment Shader\n";
	const GLchar *vshadercode = read_file("vshader.glsl");
	const GLchar *fshadercode = read_file("fshader.glsl");
	const GLchar *vcodeArray[] = {vshadercode};
	const GLchar *fcodeArray[] = {fshadercode};
	size[0] = strlen(vshadercode);
	size[1] = strlen(fshadercode);
	glShaderSource(vshader,1,vcodeArray,NULL);
	glShaderSource(fshader,1,fcodeArray,NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);
	print_compilation(vshader);
	print_compilation(fshader);
	glAttachShader(programHandle,vshader);
	glAttachShader(programHandle,fshader);

	glLinkProgram(programHandle);
	if(!print_link(programHandle))
	{
		glUseProgram(programHandle);
	}
}

int main(int argc,char *argv[])
{
	init(argc,argv);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	set_shader();
	while(!glfwWindowShouldClose(window))
	{
		display();
		glfwPollEvents();
		movement();
	}
	glfwTerminate();
	return 0;
}