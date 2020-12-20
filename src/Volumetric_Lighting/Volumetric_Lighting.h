#pragma once

#include "../Shader.h"
#include "../Camera.h"
#include "../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;



class Volumetric_Lighting
{
public:
	Volumetric_Lighting();
	void render( Shader& sceneShader);
	void drawDebugQuad(Shader& shader, unsigned int texture);
	void fillDepthBuffer(Shader& shader);

	unsigned int depthMap;
	unsigned int VolumetricLightingFBO, RBO, VolumetricLightcolorAtt;


private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int floorVAO, floorVBO;
	unsigned int quadVAO, quadVBO;
	unsigned int depthBufferFBO;

	//unsigned int depthMap;

	unsigned int cubeTextureID, floorTextureID;

	glm::mat4 lightView;
	glm::mat4 lightProjection;

	glm::vec3 lightPos;

	void initCube();
	void initFloor();
	void initDebugQuad();
	void initDepthBufferFBO();
	void initVolumetricLightingFBO();

	void drawCube(unsigned int texture);
	void drawFloor(unsigned int texture);
	
	void drawScene(Shader& shader);
};

int runVolumetricLighting();