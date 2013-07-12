#ifndef INFO
#define INFO


#include <iostream>

#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"
#include "Include/glm/glm.hpp"

void print_data();
void print_compilation(GLuint);
int print_link(GLuint);
void print_matrix(glm::mat4);

#endif