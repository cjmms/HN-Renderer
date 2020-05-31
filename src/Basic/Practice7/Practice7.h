#pragma once
#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Practice7
{
private:
	unsigned int VAO, VBO, location;

public:
	Practice7();
	void render();
};

