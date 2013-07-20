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
#include "renderer.h"



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
model model_data;


mat4 ModelView;
mat4 Model;
mat4 View;
mat4 Projection;
mat3 Normal;

vec3 position = vec3(0.0f,5.0f,40.0f);
vec3 direction = vec3(0.0f,0.0f,-1.0f);
vec3 up = vec3(0.0f,1.0f,0.0f);

int renderingMode;

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
	if (keyboard(key, scancode, action, mods, renderingMode, position, direction, up) == CLOSE_WINDOW)
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

	Model = mat4(1.0);

	// Camera is at (position), in World Space, and looks at the (direction), head is (up)
	View = glm::lookAt(position,position+direction,up);

	model_data = load_model(MODEL);


	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window,mouse_position_callback);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
}



void movement()
{   
	View = lookAt(position,position+direction,up);
	//ModelView = rotate(ModelView,rotation_value,vec3(0.0f,1.0f,0.0f));
	//ModelView = translate(ModelView,translate_value);
	//ModelView =  rotate(ModelView,rotation_value,vec3(1.0f,0.0f,0.0f));
	//ModelView = glm::scale(ModelView,scale);
}


int main(int argc,char *argv[])
{
	init(argc,argv);
	useShader(programHandle,compileShader(programHandle,vshader_name,fshader_name));
	light();
	Model = glm::mat4(1.0);
	while(!glfwWindowShouldClose(window))
	{
		renderer(programHandle, Model, View, Projection, mat3(View*Model), renderingMode, model_data, window);
		glfwPollEvents();
		movement();
	}
	glfwTerminate();
	return 0;
}