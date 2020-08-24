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
	unsigned int fbo, colorAttachment, depthAttachment;

	void initCube();

	void initQuad();

	// create a floating point FBO, GL_RGBA16
	// each component is a 16 bits floating point number
	// value in color buffer won't be clamped to 1.0
	void initFBO();

	void drawCube(Shader &shader);

	void drawQuad(Shader &shader);

	// render lighting scene to a floating point FBO
	void renderLightingScene(Shader &lightingShader);

	// render the scene(texture from FBO) to a quad
	void renderHDRScene(Shader &hdr);

public:
	HDR();
	void render(Shader& lightingShader, Shader& hdr);
};


int runHDR();

