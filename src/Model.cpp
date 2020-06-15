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
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}



// processing meshes of current node and processing child nodes recursively
void Model::processNode(aiNode* node, const aiScene* scene) 
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}


	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
		processNode(node->mChildren[i], scene);
	}
}




Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) 
{
	 
}




std::vector<Texture> Model::loadMaterialTextures(
	aiMaterial* material, aiTextureType type, std::string typeName) 
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;

}

