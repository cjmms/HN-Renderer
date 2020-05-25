// This Practice draws a rectangle
// color changes periodically
// Practices uses index buffer object(EBO)

#pragma once
#include "../../Shader.h"

class Practice3
{
private:
	unsigned int VAO, VBO, EBO;

public:
	Practice3();
	void render();
};

