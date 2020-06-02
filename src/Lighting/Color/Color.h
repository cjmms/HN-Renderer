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
	unsigned int lighting_location, container_location;

public:
	Color();
	void render(glm::mat4 view, glm::mat4 projection);
};

