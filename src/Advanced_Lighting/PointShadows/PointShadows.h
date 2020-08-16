#pragma once

// Demo of omnidirectional shadow mapping
// Stupid name, I will call it point shadow mapping(point shadows)


#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;

const unsigned int SHADOW_HEIGHT = 1024;
const unsigned int SHADOW_WIDTH = 1024;


class PointShadows
{

private:
	unsigned int floorVAO, floorVBO;
	unsigned int cubeVAO, cubeVBO;

	unsigned int quadVAO, quadVBO;

	unsigned int depthBufferFBO;

	unsigned int cubeTextureID, floorTextureID;

	glm::mat4 lightView;
	glm::mat4 lightProjection;

	void initFloor();
	void initCube();
	//void initDebugQuad();
	void initDepthBufferFBO();

	void drawFloor();
	void drawCube();
	//void drawDebugQuad(Shader& shader);
	void drawScene(Shader& shader);

	void fillDepthBuffer(Shader& shader);
	void renderScene(Shader& shader);


public:
	PointShadows();
	void render(Shader& depthBufferShader, Shader& sceneShader);
};


int runPointShadows();
