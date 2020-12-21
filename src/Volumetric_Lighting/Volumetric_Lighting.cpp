#include "Volumetric_Lighting.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <chrono>



enum BlurState
{
    NO_BLUR,
    GAUSSIAN,
    BILATERAL
};


bool enableDithering = false;
bool showUI = false;
int NB_SAMPLES = 10;




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
    sceneShader.setInt("NB_SAMPLES", NB_SAMPLES);

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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        showUI = true;
        camera.disable();
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
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

    const char* states[]{ "No Blur", "Gaussian Blur", "Bilateral Blur" };
    int state = NO_BLUR;


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
            ImGui::Begin("UI");                          // Create a window called "Hello, world!" and append into it.
              
            ImGui::Checkbox("Dithering", &enableDithering);      // Edit bools storing our window open/close state

            ImGui::SliderInt("Number of samples", &NB_SAMPLES, 10, 1000);

            ImGui::ListBox("", &state, states, IM_ARRAYSIZE(states));

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();    
        }


        camera.cameraUpdateFrameTime();

        renderer.fillDepthBuffer(depthBufferShader);
        renderer.render( sceneShader);

        // no Blur
        if (state == NO_BLUR) renderer.drawDebugQuad(debugQuadShader, renderer.VolumetricLightcolorAtt);

        // Gaussian Blur
        if (state == GAUSSIAN) renderer.GaussianBlur(GaussianBlurShader);

        // Bilateral Blur
        if (state == BILATERAL) renderer.BilateralBlur(BilateralBlurShader);

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