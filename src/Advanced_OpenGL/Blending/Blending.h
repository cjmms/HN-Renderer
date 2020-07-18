#pragma once
#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;

class Blending
{
private:
	unsigned int planeVAO, planeVBO;
	unsigned int cubeVAO, cubeVBO;

	void cubeInit();
	void planeInit();
	void drawCube(glm::mat4 mvp, Shader& shader, Texture& texture);
	void drawPlane(glm::mat4 mvp, Shader& shader, Texture& texture);

public:
	Blending();
	void renderScene(glm::mat4 view, glm::mat4 projection, Shader& shader);
};


int runBlending();


