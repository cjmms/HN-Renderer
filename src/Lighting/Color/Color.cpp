#include "Color.h"

Color::Color() 
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

	// lighting cube
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &lighting_VAO);
    glBindVertexArray(lighting_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader lighting_shader("res/Shaders/Lighting/Color/LightSource.shader");
    lighting_shader.Bind();

    lighting_location = lighting_shader.getUniformLocation("mvp");



    // container cube
    glGenVertexArrays(1, &container_VAO);
    glBindVertexArray(container_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


	Shader container_shader("res/Shaders/Lighting/Color/Container.shader");
	container_shader.Bind();

    unsigned int container_color_location = container_shader.getUniformLocation("containerColor");
    unsigned int light_color_location = container_shader.getUniformLocation("lightColor");

    glUniform3fv(container_color_location, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(light_color_location, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	container_location = container_shader.getUniformLocation("mvp");
}

void Color::render(glm::mat4 view, glm::mat4 projection) 
{

    glm::mat4 model(1.0f);
    glm::mat4 mvp = projection * view * model;

    glUniformMatrix4fv(container_location, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(container_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));

    mvp = projection * view * model;

    glUniformMatrix4fv(lighting_location, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(lighting_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}