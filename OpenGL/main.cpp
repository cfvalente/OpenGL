#include <iostream>
#include <string>

#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"

#include "Include/glm/glm.hpp"
#include "Include/glm/gtc/matrix_transform.hpp"

#include "info.h"
#include "loader.h"
#include "controls.h"
#include "shader.h"



#ifdef SPONZA
#define MODEL "Model/sponza.obj"
#else
#ifdef TEAPOT
#define MODEL "SimpleModel/teapot.obj"
#else
#ifdef CHAO
#define MODEL "SimpleModel/simples_com_chao.obj"
#else
#define MODEL "SimpleModel/simples.obj"
#endif
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

vec4 LightPosition;
vec3 La;
vec3 Ld;
vec3 Ls;

mat4 ModelView;
mat4 Model;
mat4 View;
mat4 Projection;
mat3 Normal;

vec3 position = vec3(0.0f,5.0f,40.0f);
vec3 direction = vec3(0.0f,0.0f,-1.0f);
vec3 up = vec3(0.0f,1.0f,0.0f);

int renderingMode;

GLuint *elementBufferHandle;
GLFWwindow* window;

char *vshader_name = "vshader_phong.glsl";
char *fshader_name = "fshader_phong.glsl";


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static bool a = 0;
	if (keyboard_movement(key, scancode, action, mods, renderingMode, position, direction, up) == CLOSE_WINDOW)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if(a) { cout << "A"; a = !a; }
		else { cout << "B"; a = !a; }
	}
}

void mouse_position_callback(GLFWwindow* window, double x, double y)
{
	int size_x,size_y;
	glfwGetWindowSize(window,&size_x,&size_y);
	glfwGetCursorPos(window,&x, &y);
	glfwSetCursorPos(window,(size_x/2.0),(size_y/2.0));
	mouse_movement(x,y,size_x,size_y,direction,up);
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

	renderingMode = rendering::all;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(45.0f, 1280.0f / 1024.0f, 0.1f, 500.0f);

	// Camera is at (position), in World Space, and looks at the (direction), head is (up)
	ModelView = glm::lookAt(position,position+direction,up);

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
	LightPosition = glm::vec4(0.0f,0.0f,20.0f,1.0f);
	La = glm::vec3(0.1f,0.1f,0.1f);
	//La = glm::vec3(1.0f,1.0f,1.0f);
	Ld = glm::vec3(0.8f,0.8f,0.8f);
	Ls = glm::vec3(0.3f,0.3f,0.3f);
}



void movement()
{   
	ModelView = lookAt(position,position+direction,up);
	//ModelView = rotate(ModelView,rotation_value,vec3(0.0f,1.0f,0.0f));
	//ModelView = translate(ModelView,translate_value);
	//ModelView =  rotate(ModelView,rotation_value,vec3(1.0f,0.0f,0.0f));
	//ModelView = glm::scale(ModelView,scale);
}

void display()
{
	mat4 MVP;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MVP = Projection * ModelView;
	//ModelView = View*Model; /* Usar ModelView para calcular a normal ou apenas Model? --> Apenas Model, pois a luz nao esta no espaco da camera */
	Normal = mat3(1.0f);//transpose(inverse(mat3(ModelView)));

	GLuint location = glGetUniformLocation(programHandle,"MVP");
	glUniformMatrix4fv(location, 1, GL_FALSE, &MVP[0][0]);
	location = glGetUniformLocation(programHandle,"ModelView");
	glUniformMatrix4fv(location, 1, GL_FALSE, &ModelView[0][0]);
	location = glGetUniformLocation(programHandle,"LightPosition");
	glUniform4f(location,0.0f,0.0f,40.0f,1.0f);
	location = glGetUniformLocation(programHandle,"La");
	glUniform3f(location,La[0],La[1],La[2]);
	location = glGetUniformLocation(programHandle,"Ld");
	glUniform3f(location,Ld[0],Ld[1],Ld[2]);
	location = glGetUniformLocation(programHandle,"Ls");
	glUniform3f(location,Ls[0],Ls[1],Ls[2]);
	location = glGetUniformLocation(programHandle,"renderingMode");
	glUniform1f(location,(int)renderingMode);

	/*
	GLuint blockIndex = glGetUniformBlockIndex(programHandle,"Light");
	GLint blockSize;
	glGetActiveUniformBlockiv(programHandle,blockIndex,GL_UNIFORM_BLOCK_DATA_SIZE,&blockSize);
	GLubyte *blockBuffer = (GLubyte *) malloc(blockSize);
	const GLchar *names[] = { "Position", "La", "Ld", "Ls" };
	GLuint indices[4];
	glGetUniformIndices(programHandle,4,names,indices);
	GLint offset[4];
	glGetActiveUniformsiv(programHandle,4,indices,GL_UNIFORM_OFFSET,offset);

	*/


	for(unsigned int m = 0; m < model_data.num_meshes; m++)
	{
		glBindVertexArray(vaoHandle[m]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBufferHandle[m]);
		location = glGetUniformLocation(programHandle,"Ka");
		glUniform3f(location,model_data.material_ka[model_data.mesh_material[m]][0],model_data.material_ka[model_data.mesh_material[m]][1],model_data.material_ka[model_data.mesh_material[m]][2]);
		location = glGetUniformLocation(programHandle,"Kd");
		glUniform3f(location,model_data.material_kd[model_data.mesh_material[m]][0],model_data.material_kd[model_data.mesh_material[m]][1],model_data.material_kd[model_data.mesh_material[m]][2]);
		location = glGetUniformLocation(programHandle,"Ks");
		glUniform3f(location,model_data.material_ks[model_data.mesh_material[m]][0],model_data.material_ks[model_data.mesh_material[m]][1],model_data.material_ks[model_data.mesh_material[m]][2]);
		location = glGetUniformLocation(programHandle,"Shineness");
		glUniform1f(location,model_data.material_shininess[model_data.mesh_material[m]]);

		glDrawElements(GL_TRIANGLES, 3 * model_data.num_faces[m] * sizeof(GL_UNSIGNED_INT), GL_UNSIGNED_INT, (void*)0);
	}
	glfwSwapBuffers(window);
}

int main(int argc,char *argv[])
{
	init(argc,argv);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window,mouse_position_callback);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);

	useShader(programHandle,compileShader(programHandle,vshader_name,fshader_name));


	light();
	while(!glfwWindowShouldClose(window))
	{
		display();
		glfwPollEvents();
		movement();
	}
	glfwTerminate();
	return 0;
}