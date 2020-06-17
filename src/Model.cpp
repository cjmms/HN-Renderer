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

	processVertices(vertices, mesh);
	processIndices(indices, mesh);
	processMaterials(textures, mesh, scene);

	return Mesh(vertices, indices, textures);
}




std::vector<Texture> Model::loadMaterialTextures(
	aiMaterial* material, aiTextureType type, std::string typeName) 
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		if (!textureIsLoaded(textures, str)) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, false);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
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
// if no textureCoord, return vec2(0.0f)
glm::vec2 Model::processTextureCoords(aiMesh* mesh, unsigned int i) 
{
	glm::vec2 textureCoords(0.0f);
	if (mesh->mTextureCoords[0])
	{
		textureCoords.x = mesh->mTextureCoords[0][i].x;
		textureCoords.y = mesh->mTextureCoords[0][i].y;
	}
	
	return textureCoords;
}



void Model::processVertices(std::vector<Vertex>& vertices, aiMesh* mesh)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.Position = processPosition(mesh, i);
		vertex.Normal = processNormal(mesh, i);
		vertex.TexCoords = processTextureCoords(mesh, i);

		vertices.push_back(vertex);
	}
}


void Model::processIndices(std::vector<unsigned int>& indices, aiMesh* mesh) 
{
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}



void Model::processMaterials(std::vector<Texture> &textures, aiMesh* mesh, const aiScene* scene) 
{
	// check index, make sure material exists
	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.begin(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.begin(), specular_maps.begin(), specular_maps.end());
	}
}


bool Model::textureIsLoaded(std::vector<Texture> textures, aiString &path) 
{
	for (Texture loadedTexture: loadedTextures) 
	{
		// compare path, if matchs, then texture is already loaded
		if (std::strcmp(loadedTexture.path.c_str(), path.C_Str()) == 0) 
		{
			textures.push_back(loadedTexture);
			return true;
		}
	}
	return false;
}





unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}