#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../Shader.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Camera.h"

extern Camera camera;


class DepthTest
{

private:
	// cube
	unsigned int cubeVAO, cubeVBO;

	// plane
	unsigned int planeVAO, planeVBO;

	void cubeInit();
	void planeInit();

	void drawCube(glm::mat4 mvp, Shader& shader, Texture& texture);
	void drawPlane(glm::mat4 mvp, Shader& shader, Texture& texture);

public:
	DepthTest();

	void renderScene(glm::mat4 view, glm::mat4 projection, Shader& shader);
};


int runDepthTest();
