#pragma once
#include "../../Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Color
{
private:
	unsigned int VBO;
	unsigned int container_VAO, lighting_VAO;

public:
	Color();
	void renderLightSource(glm::mat4 view, glm::mat4 projection, unsigned int mvpLocation);
	void renderContainer(glm::mat4 view, glm::mat4 projection, unsigned int mvpLocation);
};

