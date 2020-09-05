#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;

class DeferredShading
{
public:
	DeferredShading();
	void render(Shader& geometryPassShader, Shader& lightingPassShader);


private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int boxTextureID, floorTextureID;
	unsigned int FBO_G_buffer;

	unsigned int quadVAO, quadVBO;

	// three color attachments
	unsigned int gNormal, gPosition, gColor;

	void initCube();

	// create a FBO with a few attachments act like G-buffer
	void initG_buffer();

	void initQuad();

	unsigned int createColorAttachment();

	void drawCube();

	void drawQuad();

	void drawBoxes(Shader &shader); 
};




int runDeferredShading();