#include "MultipleLights.h"

MultipleLights::MultipleLights() 
{
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };



    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Light VAO 
    glGenVertexArrays(1, &Light_VAO);
    glBindVertexArray(Light_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Cube VAO
    glGenVertexArrays(1, &Cube_VAO);
    glBindVertexArray(Cube_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    lightPos[0] = glm::vec3(0.7f, 0.2f, 2.0f);
    lightPos[1] = glm::vec3(2.3f, -3.3f, -4.0f);
    lightPos[2] = glm::vec3(-4.0f, 2.0f, -12.0f);
    lightPos[3] = glm::vec3(0.0f, 0.0f, -3.0f);
}



void MultipleLights::renderLightSource(glm::mat4 view, glm::mat4 projection, Shader& shader)
{
    glBindVertexArray(Light_VAO);

    for (int i = 0; i < 4; i++) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, lightPos[i]);
        model = glm::scale(model, glm::vec3(0.2f));

        glm::mat4 mvp = projection * view * model;

        shader.setMat4("mvp", mvp);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


void MultipleLights::renderContainer(glm::mat4 view, glm::mat4 projection, Shader& shader) 
{
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    Texture diffuse_map("res/Textures/wood_container.png", PNG);
    Texture specular_map("res/Textures/steel_frame.png", PNG);

    diffuse_map.bindTexture(GL_TEXTURE0);
    specular_map.bindTexture(GL_TEXTURE1);

    
    // material uniform
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);

    // directional light uniform
    shader.setVec3("dirLight.dir", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.setVec3("dirLight.ambient", glm::vec3(0.1f));
    shader.setVec3("dirLight.diffuse", glm::vec3(0.25f));
    shader.setVec3("dirLight.specular", glm::vec3(5.0f));

    // point light uniform
    for (int i = 0; i < 4; i++) 
    {
        std::string str("pointLight[");
        shader.setVec3((str + std::to_string(i) + "].position").c_str(), lightPos[i]);
        shader.setVec3((str + std::to_string(i) + "].ambient").c_str(), glm::vec3(0.05f));
        shader.setVec3((str + std::to_string(i) + "].diffuse").c_str(), glm::vec3(0.4f));
        shader.setVec3((str + std::to_string(i) + "].specular").c_str(), glm::vec3(0.7f));
        shader.setFloat((str + std::to_string(i) + "].constant").c_str(), 1.0f);
        shader.setFloat((str + std::to_string(i) + "].linear").c_str(), 0.09f);
        shader.setFloat((str + std::to_string(i) + "].quadratic").c_str(), 0.032f);
    }

    // Spotlight uniform
    shader.setVec3("spotLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.setVec3("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outterCutoff", glm::cos(glm::radians(17.5f)));
    shader.setFloat("spotLight.epsilon", glm::cos(glm::radians(12.5f)) - glm::cos(glm::radians(17.5f)));


    glBindVertexArray(Cube_VAO);

    for (size_t i = 0; i < 10; i++)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        glm::mat4 mvp = projection * view * model;

        shader.setMat4("mvp", mvp);
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}