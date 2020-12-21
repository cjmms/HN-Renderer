#include "Volumetric_Lighting.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"



enum BlurState
{
    NO_BLUR,
    GAUSSIAN,
    BILATERAL
};

BlurState blurState = NO_BLUR;
bool enableDithering = true;
bool enableHighSampling = false;
bool showUI = false;



Volumetric_Lighting::Volumetric_Lighting()
{
    lightPos = glm::vec3(-3.0f, 3.0f, 0.0f);

    initCube();
    initFloor();
    initDebugQuad();
    initDepthBufferFBO();
    initVolumetricLightingFBO();
    createTexture(cubeTextureID, "res/Textures/container.jpg", JPG);
    createTexture(floorTextureID, "res/Textures/metal.jpg", JPG);


    lightView = glm::lookAt(lightPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));


    lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 125.0f);
}

void Volumetric_Lighting::fillDepthBuffer(Shader& shader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthBufferFBO);
    // resolution of shadow map
    glViewport(0, 0, 860, 860);

    glClear(GL_DEPTH_BUFFER_BIT);

    shader.Bind();
    shader.setMat4("projection", lightProjection);
    shader.setMat4("view", lightView);

    drawScene(shader);
}

void Volumetric_Lighting::render(Shader& sceneShader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, VolumetricLightingFBO);
    glViewport(0, 0, 1024, 1024);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sceneShader.Bind();

    sceneShader.setMat4("projection", camera.getProjectionMatrix());
    sceneShader.setMat4("view", camera.getViewMatrix());

    sceneShader.setVec3("lightPos", lightPos);
    sceneShader.setVec3("viewPos", camera.getCameraPos());

    sceneShader.setMat4("lightProjection", lightProjection);
    sceneShader.setMat4("lightView", lightView);

    sceneShader.setInt("enableDithering", enableDithering);
    sceneShader.setInt("enableHighSampling", enableHighSampling);

    sceneShader.setInt("depthMap", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    drawScene(sceneShader);
}




void Volumetric_Lighting::initVolumetricLightingFBO()
{
    // generate FBO
    glGenFramebuffers(1, &VolumetricLightingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, VolumetricLightingFBO);

    // generate FBO color attachment, bind to current FBO
    VolumetricLightcolorAtt = Texture(1024, 1024).getTextureID();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, VolumetricLightcolorAtt, 0);

    // depth buffer attachment
    // 1024 x 1024, shadow map resolution
    createDepthAttachment(volumetricLightingDepthMap, 1024, 1024);
    // depth buffer FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, volumetricLightingDepthMap, 0);

    // check if framebuffer created successfullly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Volumetric_Lighting::initDepthBufferFBO()
{
    // depth buffer attachment
    // 860 x 860, shadow map resolution
    createDepthAttachment(depthMap, 860, 860);

    // depth buffer FBO
    glGenFramebuffers(1, &depthBufferFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthBufferFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    // tell OpenGL that nothing will be drawn, no need for color buffer attachment
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Volumetric_Lighting::initFloor()
{
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    glGenBuffers(1, &floorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &floorVAO);
    glBindVertexArray(floorVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Volumetric_Lighting::initCube()
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

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Volumetric_Lighting::initDebugQuad()
{
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };


    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


void Volumetric_Lighting::drawFloor(unsigned int texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(floorVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void Volumetric_Lighting::drawCube(unsigned int texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


void Volumetric_Lighting::drawDebugQuad(Shader& shader, unsigned int texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // default FBO
    shader.Bind();

    shader.setInt("map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void Volumetric_Lighting::GaussianBlur(Shader& shader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // default FBO
    shader.Bind();

    shader.setInt("image", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, VolumetricLightcolorAtt);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void Volumetric_Lighting::BilateralBlur(Shader& shader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // default FBO
    shader.Bind();

    shader.setInt("image", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, VolumetricLightcolorAtt);

    shader.setInt("depthMap", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, volumetricLightingDepthMap);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}




void Volumetric_Lighting::drawScene(Shader& shader)
{
    shader.Bind();

    glm::mat4 model(1.0f);

    // floor
    shader.setInt("diffuseMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTextureID);
    shader.setMat4("model", glm::mat4(1.0f));
    drawFloor(floorTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -15.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 15.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);

    // middle  floor
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 28.0, 0.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, 28.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -28.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    shader.setMat4("model", model);
    drawFloor(floorTextureID);


    // cubes
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, -1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);



    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    drawCube(cubeTextureID);
}






void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        blurState = NO_BLUR;
    

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        blurState = GAUSSIAN;
    

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        blurState = BILATERAL;
    

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        enableDithering = !enableDithering;
    }

    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        enableHighSampling = !enableHighSampling;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        //enableHighSampling = !enableHighSampling;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        showUI = true;
        camera.disable();
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        //enableHighSampling = !enableHighSampling;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        showUI = false;
        camera.enable();
    }
}




int runVolumetricLighting()
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
    glfwSetMouseButtonCallback(window, mouseButton_callback);


    glfwSetKeyCallback(window, key_callback);

    if (glewInit() != GLEW_OK)
        std::cout << "init error" << std::endl;

    glEnable(GL_DEPTH_TEST);

    Shader depthBufferShader("res/Shaders/Volumetric_Lighting/depthMap.shader");
    Shader debugQuadShader("res/Shaders/Volumetric_Lighting/debugQuad.shader");
    Shader sceneShader("res/Shaders/Volumetric_Lighting/scene.shader");
    Shader GaussianBlurShader("res/Shaders/Volumetric_Lighting/GaussianBlur.shader");
    Shader BilateralBlurShader("res/Shaders/Volumetric_Lighting/BilateralBlur.shader");


    Volumetric_Lighting renderer;


    // imgui
    const char* glsl_version = "#version 130";

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);




    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Render here 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showUI)
        {
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static int f = 10.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderInt("Number of samples", &f, 10, 100);
                //ImGui::SliderFloat("int", &f, 10.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }
        }

        
        



        camera.cameraUpdateFrameTime();


        renderer.fillDepthBuffer(depthBufferShader);
        renderer.render( sceneShader);

        // no Blur
        if (blurState == NO_BLUR)
            renderer.drawDebugQuad(debugQuadShader, renderer.VolumetricLightcolorAtt);

        // Gaussian Blur
        if (blurState == GAUSSIAN)
            renderer.GaussianBlur(GaussianBlurShader);

        // Bilateral Blur
        if (blurState == BILATERAL)
            renderer.BilateralBlur(BilateralBlurShader);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwTerminate();
    return 0;
}