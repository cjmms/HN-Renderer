#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;

class Build_inVar
{
private:

	unsigned int cubeVAO, cubeVBO;

	void cubeInit();
	void drawPoints(glm::mat4 mvp, Shader& shader);

public:
	Build_inVar();
	void render(glm::mat4 view, glm::mat4 projection, Shader& shader);
};


int runBuild_inVar();

