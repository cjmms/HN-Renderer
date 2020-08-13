#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;


class ShadowMapping
{
private:
	unsigned int floorVAO, floorVBO;
	unsigned int cubeVAO, cubeVBO;

	void initFloor();
	void initCube();

	void drawFloor();
	void drawCube();


public:
	ShadowMapping();
	void render(Shader &shader);
};


int runShadowMapping();

