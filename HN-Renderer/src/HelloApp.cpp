#include "pch.h"
#include "HelloApp.h"



    void HelloTriangleApplication::run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void HelloTriangleApplication::initVulkan() {}

    void HelloTriangleApplication::mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }


void HelloTriangleApplication::initWindow()
{
        glfwInit();

        // Telling glfw that context is not created for OpenGL
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // disable window resize
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
}


void HelloTriangleApplication::cleanup() {
    glfwDestroyWindow(window);

    glfwTerminate();
}
