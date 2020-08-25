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
	void render(Shader& boxShader, Shader& lightSourceShader);

private:
	unsigned int cubeVAO, cubeVBO;

	void initCube();
	void initQuad();

	void drawCube(Shader &shader, glm::mat4 model);

	void drawLightSource(Shader &shader);

	void drawBoxes(Shader &shader);

	void renderLightingScene(Shader &boxShader, Shader &lightSourceShader);


};


int runBloom();

