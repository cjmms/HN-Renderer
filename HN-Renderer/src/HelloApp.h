#pragma once

#include "pch.h"



class HelloTriangleApplication
{
public:
    void run();

private:
    GLFWwindow* window;
    VkInstance instance;

private:
    void initVulkan();

    void mainLoop();

    void initWindow();

    void cleanup();

    void createInstance();

    void checkExtensions();
};