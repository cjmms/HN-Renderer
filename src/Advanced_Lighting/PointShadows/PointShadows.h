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



class PointShadows
{

private:
	unsigned int cubeVAO, cubeVBO;

	unsigned int depthCubemapFBO;

	unsigned int cubeTextureID, floorTextureID, depthCubemap;

	glm::mat4 lightProjection;
	std::vector<glm::mat4> lightViews;
	glm::vec3 lightPos;

	void initCube();
	//void initDebugQuad();
	void initDepthCubemapFBO();
	void initLightingMatrices();

	void drawCube(unsigned int texture);
	//void drawDebugQuad(Shader& shader);
	void drawRoom(Shader& shader);

	void fillDepthCubemap(Shader& shader);
	void renderScene(Shader& shader);


public:
	PointShadows();
	void render(Shader& depthBufferShader, Shader& sceneShader);
};


int runPointShadows();
