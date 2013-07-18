#include <iostream>
#include <string>

#include "Include/glm/gtc/matrix_transform.hpp"

#include "info.h"
#include "renderer.h"

using namespace glm;


vec4 LightPosition;
vec3 La;
vec3 Ld;
vec3 Ls;

/* Cuida da inicializacao da luz */
void light()
{
	LightPosition = glm::vec4(0.0f,0.0f,20.0f,1.0f);
	La = glm::vec3(0.1f,0.1f,0.1f);
	//La = glm::vec3(1.0f,1.0f,1.0f);
	Ld = glm::vec3(0.8f,0.8f,0.8f);
	Ls = glm::vec3(0.3f,0.3f,0.3f);
}

void renderer(GLuint programHandle, mat4 Model, mat4 View, mat4 Projection, mat3 Normal, int renderingMode, model model_data, GLuint *vaoHandle, GLuint *elementBufferHandle, GLFWwindow* window)
{
	mat4 MVP, ModelView;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ModelView = View * Model;
	MVP = Projection * ModelView;

	Normal = mat3(ModelView);

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
	glUniform1f(location, renderingMode);

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