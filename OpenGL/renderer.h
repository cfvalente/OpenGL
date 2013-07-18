#ifndef RENDERER
#define RENDERER


#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"

#include "Include/glm/glm.hpp"
#include "loader.h"

void light();
void renderer(GLuint programHandle, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection, glm::mat3 Normal, int renderingMode, model model_data, GLuint *vaoHandle, GLuint *elementBufferHandle, GLFWwindow* window);

#endif
