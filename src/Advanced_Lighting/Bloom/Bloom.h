#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;


class Bloom
{
public:
	Bloom();
	void render(Shader& boxShader, Shader& lightSourceShader, Shader& bloomShader);

private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int boxTextureID;
	unsigned int floorTextureID;

	unsigned int quadVAO, quadVBO;

	unsigned int fbo;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	unsigned int colorBuffers[2];

	void initCube();
	void initQuad();

	// init a fbo to render HDR scene
	// no depth buffer and stencil buffer
	// contains 2 color buffer, one for lighting scene, one for lighting extraction
	void initFBO();

	void drawCube(Shader &shader, glm::mat4 model);

	void drawQuad();

	void drawLightSource(Shader &shader);

	void drawBoxes(Shader &shader);

	void renderBloomScene(Shader &bloomShader);

	void renderLightingScene(Shader &boxShader, Shader &lightSourceShader);


};


int runBloom();

