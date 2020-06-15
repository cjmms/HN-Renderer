#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"


class Model
{
public:
	Model(char *path);
	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(
		aiMaterial* material, aiTextureType type, std::string typeName);

	glm::vec3 processPosition(aiMesh* mesh, unsigned int i);
	glm::vec3 processNormal(aiMesh* mesh, unsigned int i);
	glm::vec2 processTextureCoords(aiMesh* mesh, unsigned int i);
	void processVertex(std::vector<Vertex> &vertices, aiMesh* mesh);
};

