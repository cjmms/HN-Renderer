#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>


struct Vectex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitengent;
};

struct Texture {
	unsigned int id;
	std::string type;	// diffuse or specular
	std::string path;
};



class Mesh
{

public:
	std::vector<Vectex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

	Mesh(std::vector<Vectex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader &shader);

private:
	unsigned int VBO, EBO;
	void setup();
};

