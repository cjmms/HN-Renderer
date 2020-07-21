#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "Practice.h"


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
    camera.updateCameraDirection((float)xpos, (float)ypos);
}


int main(void) 
{
    //runMultipleLights();

    //  Advanced OpenGL
    //runDepthTest();
    //runStencilTest();
    //runBlending();
    runCulling();
}
