#include "Practice3.h"


Practice3::Practice3 ()
{
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f,  // bottom left
	   -0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
	    1, 2, 3    // second triangle
	};

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shader("res/Shaders/Practice3.shader");
	shader.Bind();

	int uniformLocation = shader.getUniformLocation("updatedColor");
	glUniform3f(uniformLocation, 0.5f, 0.5f, 0.5f);
}

void Practice3::render() 
{
	glBindVertexArray(VAO);

	// The last arg is 0 since we do provide an EBO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

