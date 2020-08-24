#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;


class HDR
{
private:

	unsigned int cubeVAO, cubeVBO, cubeTex;
	unsigned int quadVAO, quadVBO;

	void initCube();

	void initQuad();

	// render lighting scene to a floating point FBO
	void renderLightingScene(Shader &lightingShader);

	// render the scene(texture from FBO) to a quad
	void renderHDRScene(Shader &hdr);

public:
	HDR();
	void render(Shader& lightingShader, Shader& hdr);
};


int runHDR();

