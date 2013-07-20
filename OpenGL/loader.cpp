#include <iostream>
#include <string>
#include <stdio.h>
#include "loader.h"
#include "Include/assimp/Importer.hpp"
#include "Include/assimp/scene.h"
#include "Include/assimp/postprocess.h"



#ifdef LOADER_DEBBUG
# define print(x) std::cout << x
#else
# define print(x) do {} while (0)
#endif

//TODO: Função incompleta! 1 mesh OK, sem texturas, sem cores, sem materiais
struct model load_model(std::string pFile)
{
	struct model result;
	Assimp::Importer importer;
	const aiMesh *mesh = NULL;
	const aiScene* scene = importer.ReadFile( pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	result.success = 1;
	if( !scene)
	{
		result.success = 0;
		result.error_message = (std::string) importer.GetErrorString();
		return result;
	}
	mesh = scene->mMeshes[0];
	if (!mesh)
	{
		result.success = 0;
		result.error_message = (std::string) "Error Finding Model In file.  Did you export an empty scene?";
		return result;
	}


	result.num_meshes = scene->mNumMeshes;
	result.material_ka = new float*[result.num_meshes];
	result.material_kd = new float*[result.num_meshes];
	result.material_ks = new float*[result.num_meshes];
	result.material_shininess = new float[result.num_meshes];
	result.num_materials = scene->mNumMaterials;


	/* Carrega os materiais */
	for(unsigned int i = 0; i<scene->mNumMaterials; i++)
	{
		aiColor3D color (0.f,0.f,0.f);

		result.material_ka[i] = new float[3];
		result.material_ks[i] = new float[3];
		result.material_kd[i] = new float[3];

		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE,color);

		result.material_kd[i][0] = color.r;
		result.material_kd[i][1] = color.g;
		result.material_kd[i][2] = color.b;

		print("kd " << i << ": " << result.material_kd[i][0] << " " << result.material_kd[i][1] << " " << result.material_kd[i][2] << "\n");

		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT,color);

		result.material_ka[i][0] = color.r;
		result.material_ka[i][1] = color.g;
		result.material_ka[i][2] = color.b;

		print("ka " << i << ": " << result.material_ka[i][0] << " " << result.material_ka[i][1] << " " << result.material_ka[i][2] << "\n");

		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR,color);

		result.material_ks[i][0] = color.r;
		result.material_ks[i][1] = color.g;
		result.material_ks[i][2] = color.b;

		print("ks " << i << ": " << result.material_ks[i][0] << " " << result.material_ks[i][1] << " " << result.material_ks[i][2] << "\n");\

		scene->mMaterials[i]->Get(AI_MATKEY_SHININESS,result.material_shininess[i]);;

	}

	/* Carrega o modelo */
	result.num_vertices = new unsigned int[scene->mNumMeshes];
	result.normal_data = new float*[result.num_meshes];
	result.position_data = new float*[result.num_meshes];
	result.mesh_material = new unsigned [scene->mNumMaterials];

	result.num_faces = new unsigned int[scene->mNumMeshes];
	result.face_data = new unsigned int*[scene->mNumMeshes];

	for(unsigned int m = 0; m < scene->mNumMeshes; m++)
	{

		mesh = scene->mMeshes[m];

		result.num_vertices[m] = mesh->mNumVertices;
		result.mesh_material[m] = mesh->mMaterialIndex;
		result.normal_data[m] = new float[3*mesh->mNumVertices];
		result.position_data[m] = new float[3*mesh->mNumVertices];

		result.num_faces[m] = mesh->mNumFaces;
		result.face_data[m] = new unsigned int[3*mesh->mNumFaces];

		for(unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			result.position_data[m][3*i] = mesh->mVertices[i].x;
			result.position_data[m][3*i+1] = mesh->mVertices[i].y;
			result.position_data[m][3*i+2] = mesh->mVertices[i].z;

			result.normal_data[m][3*i] = mesh->mNormals[i].x;
			result.normal_data[m][3*i+1] = mesh->mNormals[i].y;
			result.normal_data[m][3*i+2] = mesh->mNormals[i].z;
			print("Mesh: " << m << "  " << "Vert: " << i << "  " << result.position_data[m][3*i] << "  " << result.position_data[m][3*i+1] << "  " << result.position_data[m][3*i+2] << "\n");
		}

		/* Carrega as faces */
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
		{
			if (mesh->mFaces[i].mNumIndices == 3)
			{
				result.face_data[m][3*i] = mesh->mFaces[i].mIndices[0];
				result.face_data[m][3*i+1] = mesh->mFaces[i].mIndices[1];
				result.face_data[m][3*i+2] = mesh->mFaces[i].mIndices[2];

				print("Mesh: " << m << "  " << "Face: " << i << "  " << result.face_data[m][3*i] << "  " << result.face_data[m][3*i+1] << "  " << result.face_data[m][3*i+2] << "\n");


			}
			else
			{
				result.success = 0;
				result.error_message = (std::string) "Error parsing Faces. Try to Re-Export model from 3d package! 3 vertices per face!";
				return result;
			}
		}
	}

	result.vaoHandle = new GLuint[result.num_meshes];
	result.elementBufferHandle = new GLuint[result.num_meshes];
	for(unsigned int m = 0; m<result.num_meshes; m++)
	{
		GLuint vboHandles[2];
		glGenBuffers(1,&vboHandles[0]);
		GLuint positionBufferHandle = vboHandles[0];
		glGenBuffers(1,&vboHandles[1]);
		GLuint normalBufferHandle = vboHandles[1];

		glGenBuffers(1, &result.elementBufferHandle[m]);

		glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
		glBufferData(GL_ARRAY_BUFFER,3*result.num_vertices[m]*sizeof(float),result.position_data[m],GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
		glBufferData(GL_ARRAY_BUFFER,3*result.num_vertices[m]*sizeof(float),result.normal_data[m],GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.elementBufferHandle[m]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * result.num_faces[m] * sizeof(GL_UNSIGNED_INT), result.face_data[m], GL_STATIC_DRAW);

		glGenVertexArrays(1,&result.vaoHandle[m]);
		glBindVertexArray(result.vaoHandle[m]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte *)NULL);

	}
	return result;
}