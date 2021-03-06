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

	unsigned int quadVAO, quadVBO;

	unsigned int depthBufferFBO;

	unsigned int cubeTextureID, floorTextureID, depthMap;

	glm::mat4 lightView;
	glm::mat4 lightProjection;

	void initFloor();
	void initCube();
	void initDebugQuad();
	void initDepthBufferFBO();

	void drawFloor();
	void drawCube();
	void drawDebugQuad(Shader& shader);
	void drawScene(Shader &shader);

	void fillDepthBuffer(Shader &shader);
	void renderScene(Shader& shader);


public:
	ShadowMapping();
	void render(Shader &depthBufferShader, Shader &sceneShader, Shader &debugQuadShader);
};


int runShadowMapping();

