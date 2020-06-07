#pragma once
#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirectionalLight
{
private:
	unsigned int VBO, VAO;

public:
	DirectionalLight();
	void renderContainer(glm::mat4 view, glm::mat4 projection, Shader& shader);
};

