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



// process vertices, indices, textures from aiMesh object
// copy all these data to my Mesh object and return it
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	processVertex(vertices, mesh);


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
		//texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;

}




glm::vec3 Model::processPosition(aiMesh* mesh, unsigned int i) 
{
	glm::vec3 position;
	position.x = mesh->mVertices[i].x;
	position.y = mesh->mVertices[i].y;
	position.z = mesh->mVertices[i].z;
	return position;
}


glm::vec3 Model::processNormal(aiMesh* mesh, unsigned int i)
{
	glm::vec3 normal;
	normal.x = mesh->mNormals[i].x;
	normal.y = mesh->mNormals[i].y;
	normal.z = mesh->mNormals[i].z;
	return normal;
}

// only care about the first texture set, check if mesh contains texture
glm::vec2 Model::processTextureCoords(aiMesh* mesh, unsigned int i) 
{
	glm::vec2 textureCoords;
	if (mesh->mTextureCoords[0])
	{
		textureCoords.x = mesh->mTextureCoords[0][i].x;
		textureCoords.y = mesh->mTextureCoords[0][i].y;
	}
	else
		textureCoords = glm::vec2(0.0f);
	
	return textureCoords;
}



void Model::processVertex(std::vector<Vertex>& vertices, aiMesh* mesh)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process position
		vertex.Position = processPosition(mesh, i);

		// process normal
		vertex.Normal = processNormal(mesh, i);

		// process texture coordinates
		vertex.TexCoords = processTextureCoords(mesh, i);

		vertices.push_back(vertex);
	}
}