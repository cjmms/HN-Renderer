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

	unsigned int depthBufferFBO;

	unsigned int cubeTextureID, floorTextureID;

	glm::mat4 lightView;
	glm::mat4 lightProjection;

	void initCube();
	//void initDebugQuad();
	void initDepthBufferFBO();

	void drawCube(unsigned int texture);
	//void drawDebugQuad(Shader& shader);
	void drawRoom(Shader& shader);

	void fillDepthBuffer(Shader& shader);
	void renderScene(Shader& shader);


public:
	PointShadows();
	void render(Shader& depthBufferShader, Shader& sceneShader);
};


int runPointShadows();
