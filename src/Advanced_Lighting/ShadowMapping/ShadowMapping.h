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

	void drawFloor(Shader &shader);
	void drawCube();


public:
	ShadowMapping();
	void render(glm::mat4 view, glm::mat4 projection, Shader &shader);
};


int runShadowMapping();

