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

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	void initCube();
	void initQuad();

	void drawCube(Shader &shader, glm::mat4 model);

	void drawQuad();

	void drawLightSource(Shader &shader);

	void drawBoxes(Shader &shader);

	void renderBloomScene(Shader &bloomShader);

	void renderLightingScene(Shader &boxShader, Shader &lightSourceShader);


};


int runBloom();

