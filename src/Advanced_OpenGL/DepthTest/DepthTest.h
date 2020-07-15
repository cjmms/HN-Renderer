#pragma once

#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class DepthTest
{

private:
	// cube
	unsigned int cubeVAO, cubeVBO;

	// plane
	unsigned int planeVAO, planeVBO;

	void cubeInit();
	void planeInit();

	void drawCube(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Shader& shader);
	void drawPlane(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Shader& shader);

public:
	DepthTest();

	void renderScene(glm::mat4 view, glm::mat4 projection, Shader& shader);
};

