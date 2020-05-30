#pragma once
#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Practice6
{
private:
	unsigned int VAO, VBO, EBO;
	unsigned int location;

public:
	Practice6();
	void render();
};

