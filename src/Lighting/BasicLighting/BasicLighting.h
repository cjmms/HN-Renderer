#pragma once
#include "../../Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class BasicLighting
{
private:
	unsigned int VBO;
	unsigned int Cube_VAO, Lighting_VAO;
	glm::vec3 lightPos;

public:
	BasicLighting();
	void renderLightSource(glm::mat4 view, glm::mat4 projection, Shader& shader);
	void renderContainer(glm::mat4 view, glm::mat4 projection, Shader& shader);
};

