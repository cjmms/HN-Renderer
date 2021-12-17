// HN-Renderer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "HelloApp.h"


int main()
{
    HelloTriangleApplication app;

    try
    {
        app.run();
    } 
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

