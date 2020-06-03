
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include "Practice.h"
#include "Camera.h"
#include "Shader.h"


Camera camera;


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.setCameraKey(window);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.zoomIn((float) yoffset);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.updateCameraDirection(xpos, ypos);
}



int main(void)
{
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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

    Shader lightSourceShader("res/Shaders/Lighting/BasicLighting/LightSource.shader");
    Shader containerShader("res/Shaders/Lighting/BasicLighting/Cube.shader");

    BasicLighting renderer;


    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // Render here 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.cameraUpdateFrameTime();

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
  
        containerShader.Bind();
        containerShader.setVec3("CubeColor", glm::vec3(1.0f, 0.5f, 0.31f));
        containerShader.setVec3("LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        renderer.renderContainer(view, projection, containerShader);


        lightSourceShader.Bind();
        renderer.renderLightSource(view, projection, lightSourceShader);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

