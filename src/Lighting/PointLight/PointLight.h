#pragma once
#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class PointLight
{
private:
	unsigned int VBO;
	unsigned int Light_VAO, Cube_VAO;

	glm::vec3 LightPos;

public:
	PointLight();
	void renderContainer(glm::mat4 view, glm::mat4 projection, Shader &shader);
	void renderLightSource(glm::mat4 view, glm::mat4 projection, Shader &shader);
};

