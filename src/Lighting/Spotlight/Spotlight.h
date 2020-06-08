#pragma once

#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Spotlight
{
private:
	unsigned int VBO;
	unsigned int Cube_VAO;


public:
	Spotlight();
	void renderContainer(glm::mat4 view, glm::mat4 projection, Shader& shader);

};

