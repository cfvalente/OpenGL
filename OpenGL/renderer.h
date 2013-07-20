#ifndef RENDERER
#define RENDERER


#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"

#include "Include/glm/glm.hpp"
#include "loader.h"

void light();
void renderer(GLuint programHandle, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection, glm::mat3 Normal, int renderingMode, model model_data, GLFWwindow* window);
void uniformToShader4mat(GLuint programHandle,char *var, glm::mat4 value);
void uniformToShader3mat(GLuint programHandle,char *var, glm::mat3 value);
void uniformToShader4f(GLuint programHandle,char *var, glm::vec4 value);
void uniformToShader3f(GLuint programHandle,char *var, glm::vec3 value);
void uniformToShader1f(GLuint programHandle,char *var, float value);

#endif
