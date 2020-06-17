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
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// Texture unit naming convention: texture_diffuse1, texture_diffuse2, texture_specular1 ......
	std::vector<Texture> loadMaterialTextures(
		aiMaterial* material, aiTextureType type, std::string typeName);

	glm::vec3 processPosition(aiMesh* mesh, unsigned int i);
	glm::vec3 processNormal(aiMesh* mesh, unsigned int i);
	glm::vec2 processTextureCoords(aiMesh* mesh, unsigned int i);


	void processVertices(std::vector<Vertex> &vertices, aiMesh* mesh);
	void processIndices(std::vector<unsigned int> &indices, aiMesh* mesh);
	void processMaterials(std::vector<Texture> &textures, aiMesh* mesh, const aiScene* scene);

	// check if texture is loaded, if loaded, loaded from loadedTexture
	// return true if texture is loaded
	bool textureIsLoaded(std::vector<Texture>textures, aiString &path);
};

