#ifndef SHADER
#define SHADER

#include "Include/glew/glew.h"



void printShader(int shaderIndex);
void useShader(GLuint &programHandle, int shaderIndex);
int compileShader(GLuint &programHandle, char *vshader_name, char *fshader_name);

#endif