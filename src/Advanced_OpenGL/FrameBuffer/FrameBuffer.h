#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;

enum SceneType
{
	ORIGIN,
	INVERSE,
	GERY,
	EDGE,
	DEFAULT
};

class FrameBuffer
{
private:
	unsigned int FBO, RBO, colorAttachment;
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	unsigned int quadVAO, quadVBO;

	void cubeInit();
	void planeInit();
	void quadInit();

	void drawCube(glm::mat4 mvp, Shader& shader, Texture& texture);
	void drawPlane(glm::mat4 mvp, Shader& shader, Texture& texture);

public:
	FrameBuffer();
	void drawScene(glm::mat4 view, glm::mat4 projection, Shader& shader);
	void render(glm::mat4 view, glm::mat4 projection, Shader& shader, Shader& screenShader);
};

void process(GLFWwindow* window, SceneType& type);

int runFrameBuffer();

