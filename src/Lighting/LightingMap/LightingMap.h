#pragma once
#include "../../Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LightingMap
{
private:
	unsigned int VBO;
	unsigned int light_VAO, cube_VAO;
	glm::vec3 LightPos;

public:
	LightingMap();
	void renderContainer(glm::mat4 view, glm::mat4 projection, Shader& shader);
	void renderLightSource(glm::mat4 view, glm::mat4 projection, Shader& shader);
};

