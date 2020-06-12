#include "Mesh.h"


Mesh::Mesh(std::vector<Vectex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) 
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	setup();
}


void Mesh::draw(Shader& shader) 
{
}


void Mesh::setup() 
{

}