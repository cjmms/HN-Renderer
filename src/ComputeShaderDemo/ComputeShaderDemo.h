#pragma once


#include "../Shader.h"
#include "../Camera.h"
#include "../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Camera camera;





class ComputeShaderDemo
{

public:
	GLuint tex_output;
	GLuint ray_program;
	unsigned int VAO, VBO;

	ComputeShaderDemo();
	void render(glm::mat4 view, glm::mat4 projection, Shader& shader, Texture& texture);
};


int runComputeShaderDemo();

