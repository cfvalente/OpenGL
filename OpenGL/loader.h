#ifndef LOADER
#define LOADER

struct model
{
	float **position_data;
	float **normal_data;
	float **material_ks;
	float **material_ka;
	float **material_kd;
	unsigned int *mesh_material;
	unsigned int **face_data;
	unsigned int *num_vertices;
	unsigned int *num_faces;
	unsigned int num_materials;
	unsigned int num_meshes;
	bool success;
	std::string error_message;
};


/// <summary>
/// Loads a model.
/// <para> name="pFile" --> The name of the file that contains the model.</para>
/// </summary>
struct model load_model(std::string pFile);

#endif