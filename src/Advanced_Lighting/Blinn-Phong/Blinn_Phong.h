#pragma once

#include "../../Shader.h"
#include "../../Camera.h"
#include "../../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;



class Blinn_Phong
{
public:
	Blinn_Phong();
	void render(glm::mat4 view, glm::mat4 projection, Shader &shader, Texture &texture);
};


int runBlinnPhong();

