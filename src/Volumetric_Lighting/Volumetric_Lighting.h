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
	void GaussianBlur(Shader& shader);
	void BilateralBlur(Shader& shader);
	void RayMarching(Shader& shader);

	unsigned int lightSpaceDepthMap;
	unsigned int VolumetricLightingFBO, RBO, VolumetricLightcolorAtt;
	unsigned int volumetricLightingDepthMap;
	unsigned int RayMarchingFBO, RayMarchingColorAtt;
	


private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int floorVAO, floorVBO;
	unsigned int quadVAO, quadVBO;
	unsigned int depthBufferFBO;
	//unsigned int RayMarchingFBO, RayMarchingColorAtt;


	unsigned int cubeTextureID, floorTextureID;

	glm::mat4 lightView;
	glm::mat4 lightProjection;

	glm::vec3 lightPos;

	void initCube();
	void initFloor();
	void initDebugQuad();
	void initDepthBufferFBO();
	void initVolumetricLightingFBO();
	void initRayMarchingFBO();

	void drawCube(unsigned int texture);
	void drawFloor(unsigned int texture);
	
	void drawScene(Shader& shader);
};

int runVolumetricLighting();