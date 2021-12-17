#pragma once

#include "pch.h"



class HelloTriangleApplication
{
public:
    void run();

private:
    GLFWwindow* window;

private:
    void initVulkan();

    void mainLoop();

    void initWindow();

    void cleanup();
};