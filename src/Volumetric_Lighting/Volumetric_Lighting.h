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




private:
	unsigned int cubeVAO, cubeVBO;
	unsigned int floorVAO, floorVBO;

	unsigned int depthCubemapFBO;

	unsigned int cubeTextureID, floorTextureID, depthCubemap;

	glm::mat4 lightProjection;
	std::vector<glm::mat4> lightViews;
	glm::vec3 lightPos;

	void initCube();
	void initFloor();
	//void initDebugQuad();
	void initLightingMatrices();

	void drawCube(unsigned int texture);
	void drawFloor(unsigned int texture);
	//void drawDebugQuad(Shader& shader);


	void renderScene(Shader& shader);


};

int runVolumetricLighting();

