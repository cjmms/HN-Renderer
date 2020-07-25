#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;

class SkyBox
{
private:
	unsigned int cubemapVAO, cubemapVBO;
    unsigned int cubemapTexture;

	void cubemapInit();
	void drawCubemap();

    std::vector<std::string> faces
    {
        "res/Textures/skybox/right.jpg",
        "res/Textures/skybox/left.jpg",
        "res/Textures/skybox/top.jpg",
        "res/Textures/skybox/bottom.jpg",
        "res/Textures/skybox/front.jpg",
        "res/Textures/skybox/back.jpg"
    };

public:
	SkyBox();
	void render(glm::mat4 view, glm::mat4 projection, Shader& shader);
};


int runSkyBox();
