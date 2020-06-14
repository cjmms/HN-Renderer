#include "Model.h"


Model::Model(char* path) 
{
	loadModel(path);
}

void Model::draw(Shader& shader) 
{
	for (Mesh mesh: meshes)
		mesh.draw(shader);
}


void Model::loadModel(std::string path) 
{

}


