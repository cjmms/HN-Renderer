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
	void render(Shader &shader);


private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int boxTextureID, floorTextureID;
	unsigned int FBO_G_buffer;

	// three color attachments
	unsigned int gNormal, gPosition, gColor;

	void initCube();
	unsigned int createColorAttachment();

	void drawCube();

	void drawBoxes(Shader &shader);

	// create a FBO with a few attachments act like G-buffer
	void initG_buffer();

};




int runDeferredShading();