#include "PointShadows.h"


PointShadows::PointShadows() 
{
    // light position init to origin
    lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	initCube();

	initDepthCubemapFBO();

    initLightingMatrices();
}



void PointShadows::initCube()
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    createTexture(cubeTextureID, "res/Textures/container.jpg", JPG);
    createTexture(floorTextureID, "res/Textures/wood.jpg", JPG);
}



void PointShadows::initDepthCubemapFBO()
{
    // resolution of cubemap texture: 860 X 860
    depthCubemap = createDepthCubemap(860, 860);
    
    glGenFramebuffers(1, &depthCubemapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void PointShadows::initLightingMatrices()
{
    // init view matrix
    // using perspective projection since this demo is using point light
    // angle must be 90 degree to cover a cube
    float aspect = (float)860 / (float)860;
    float near = 1.0f;
    float far = 25.0f;
    lightProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);

    // init 6 projection matrix
    lightViews.push_back(glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightViews.push_back(glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightViews.push_back(glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)));
    lightViews.push_back(glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    lightViews.push_back(glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightViews.push_back(glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}



void PointShadows::drawCube(unsigned int texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}



void PointShadows::drawRoom(Shader &shader)
{
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

    shader.Bind();
    shader.setMat4("model", model);

    // rendering inside of cube, cull face
    glDisable(GL_CULL_FACE);

    // using this uniform to reverse normal
    // since the light source is inside the room
    shader.setInt("reverse_normal", 1.0f);
    drawCube(floorTextureID);
    shader.setInt("reverse_normal", 0.0f);

    glEnable(GL_CULL_FACE);
}



void PointShadows::fillDepthCubemap(Shader& shader)
{
    glViewport(0, 0, 860, 860);
    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);

    glClear(GL_DEPTH_BUFFER_BIT);
    shader.Bind();

    for (unsigned int i = 0; i < 6; ++i)
        shader.setMat4("lightViews[" + std::to_string(i) + "]", lightViews[i]);

    shader.setMat4("lightProjection", lightProjection);
    shader.setVec3("lightPos", lightPos);
    shader.setFloat("farPlane", 25.0f);

    renderScene(shader);
}



void PointShadows::renderScene(Shader& shader)
{
    shader.Bind();

    // bind cube room texture
    drawRoom(shader);

    // draw 5 cubes
    glm::mat4 model(1.0f);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);
}



void PointShadows::render(Shader& depthBufferShader, Shader& sceneShader)
{
    // generate shadow map
    fillDepthCubemap(depthBufferShader);

    // render with cubemap
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 1024);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sceneShader.Bind();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    sceneShader.setMat4("projection", camera.getProjectionMatrix());
    sceneShader.setMat4("view", camera.getViewMatrix());
 
    sceneShader.setVec3("lightPos", lightPos);
    sceneShader.setVec3("viewPos", camera.getCameraPos());

    sceneShader.setFloat("farPlane", 25.0f);

    renderScene(sceneShader);
    
}



int runPointShadows()
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

    if (glewInit() != GLEW_OK)
        std::cout << "init error" << std::endl;

    glEnable(GL_DEPTH_TEST);

    Shader depthBufferShader("res/Shaders/Advanced_Lighting/PointShadows/simpleDepthCubemap.shader");

    Shader sceneShader("res/Shaders/Advanced_Lighting/PointShadows/pointShadow.shader");

    sceneShader.Bind();
    sceneShader.setInt("diffuseMap", 0);
    sceneShader.setInt("depthMap", 1);

    PointShadows renderer;

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Render here 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.cameraUpdateFrameTime();

        renderer.render(depthBufferShader, sceneShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}