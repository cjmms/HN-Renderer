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



	void initCube();

	void drawCube();

};




int runDeferredShading();