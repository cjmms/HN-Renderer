#pragma once
#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Practice5
{
private:
	unsigned int VAO, VBO, EBO;
	glm::mat4 trans;
	unsigned int location; // uniform location

public:
	Practice5();
	void render();
};

