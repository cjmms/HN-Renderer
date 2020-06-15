#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;	// diffuse or specular
	std::string path;
};



// Texture unit naming convention: texture_diffuse1, texture_diffuse2, texture_specular1 ......
class Mesh
{

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// bind texture, bind VAO, draw
	void draw(Shader &shader);

private:
	unsigned int VBO, EBO;
	void setup();
	// read date from textures and bind textures
	void bindTexture(Shader &shader);
};

