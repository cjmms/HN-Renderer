#include "DeferredShading.h"

DeferredShading::DeferredShading()
{
    initCube();
    initG_buffer();
    initQuad();

    // diffuse map
    createTexture(boxTextureID, "res/Textures/wood_container.png", true);
    createTexture(floorTextureID, "res/Textures/wood.jpg", true);
    createTexture(boxSpecID, "res/Textures/steel_frame.png", true);

    const unsigned int NR_LIGHTS = 32;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = ((rand() % 100) / 100.0) * 8.0 - 3.0;
        float yPos = ((rand() % 100) / 100.0) * 8.0 - 4.0;
        float zPos = ((rand() % 100) / 100.0) * 8.0 - 3.0;
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }
}



void DeferredShading::initCube()
{
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };


    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    glBindVertexArray(0);
}


void DeferredShading::initQuad()
{
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void DeferredShading::drawQuad()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}



unsigned int DeferredShading::createColorAttachment()
{
    unsigned int attachmentID;
    glGenTextures(1, &attachmentID);
    glBindTexture(GL_TEXTURE_2D, attachmentID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return attachmentID;
}



void DeferredShading::initG_buffer()
{
    glGenFramebuffers(1, &FBO_G_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_G_buffer);

    // create and bind three color attachments
    gPosition = createColorAttachment();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    gNormal = createColorAttachment();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    gColor = createColorAttachment();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);      // tell openGL number of color attachments

    // create and bind depth buffer attachment
    unsigned int depthAttachment;
    glGenRenderbuffers(1, &depthAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, depthAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);

    // check for FBO completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "G-buffer FBO didn't init successfully" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // unbind
}



void DeferredShading::drawCube()
{
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}



void DeferredShading::drawBoxes(Shader& shader)
{
    shader.Bind();
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setInt("diffuseMap", 0);
    shader.setInt("specularMap", 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, boxSpecID);


    glActiveTexture(GL_TEXTURE0);

    // create one large cube that acts as the floor
    glBindTexture(GL_TEXTURE_2D, floorTextureID);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0));
    model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
    shader.setMat4("model", model);
    drawCube();

    // then create multiple cubes as the scenery
    glBindTexture(GL_TEXTURE_2D, boxTextureID);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader.setMat4("model", model);
    drawCube();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.7f, 4.0));
    model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(1.0));
    shader.setMat4("model", model);
    drawCube();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, -3.0));
    model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader.setMat4("model", model);
    drawCube();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube();

    shader.unBind();
}


void DeferredShading::geometryPass(Shader& shader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_G_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    shader.Bind();
    drawBoxes(shader);
    shader.unBind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




void DeferredShading::lightingPass(Shader& shader)
{
    // using G-buffer as textures and do all the lighting calculation
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Bind();

    shader.setInt("gPosition", 0);
    shader.setInt("gNormal", 1);
    shader.setInt("AlbedoSpec", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColor);

    shader.setVec3("viewPos", camera.getCameraPos());

    for (unsigned int i = 0; i < lightPositions.size(); ++i)
    {
        shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);

        // atteunation factors
        const float linear = 0.7f;
        const float quadratic = 1.8f;

        shader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        shader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);

        const float maxI = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);

        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (1.0f - (256.0f / 5.0f) * maxI))) / (2.0f * quadratic);
        shader.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }

    drawQuad();
}



void DeferredShading::render(Shader &geometryPassShader, Shader &lightingPassShader, Shader &lightSourceShader)
{
    // render objects and pass all geometry info into a G-buffer
    geometryPass(geometryPassShader);

    // do lighting calculations in a quad
    lightingPass(lightingPassShader);

    // copy depth buffer from geometry pass to current FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_G_buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    glBlitFramebuffer(
        0, 0, 1024, 1024, 0, 0, 1024, 1024, GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // render light source
    lightSourceShader.Bind();

    lightSourceShader.setMat4("view", camera.getViewMatrix());
    lightSourceShader.setMat4("projection", camera.getProjectionMatrix());

    for (unsigned int i = 0; i < lightPositions.size(); ++i)
    {
        glm::mat4 model(1.0f);

        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.125f));
        
        lightSourceShader.setMat4("model", model);
        lightSourceShader.setVec3("color", lightColors[i]);

        drawCube();
    }

}



int runDeferredShading()
{
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(1024, 1024, "Vanilla", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //glfwSetKeyCallback(window, key_callback);

    if (glewInit() != GLEW_OK)
        std::cout << "init error" << std::endl;

    glEnable(GL_DEPTH_TEST);

    Shader geometryPassShader("res/Shaders/Advanced_Lighting/DeferredShading/geometryPass.shader");
    Shader lightingPassShader("res/Shaders/Advanced_Lighting/DeferredShading/lightingPass.shader");
    Shader lightingSourceShader("res/Shaders/Advanced_Lighting/DeferredShading/lightSource.shader");

    DeferredShading renderer;

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Render here 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.cameraUpdateFrameTime();

        renderer.render(geometryPassShader, lightingPassShader, lightingSourceShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}