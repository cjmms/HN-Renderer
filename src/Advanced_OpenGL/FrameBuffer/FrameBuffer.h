#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;


class FrameBuffer
{
private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;

	void cubeInit();
	void planeInit();

	void drawCube(glm::mat4 mvp, Shader& shader, Texture& texture);
	void drawPlane(glm::mat4 mvp, Shader& shader, Texture& texture);

public:
	FrameBuffer();
	void render(glm::mat4 view, glm::mat4 projection, Shader& shader);
};


int runFrameBuffer();

