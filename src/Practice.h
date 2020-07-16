#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"

extern Camera camera;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#include "Basic/Practice1/Practice1.h"
#include "Basic/Practice2/Practice2.h"
#include "Basic/Practice3/Practice3.h"
#include "Basic/Practice4/Practice4.h"
#include "Basic/Practice5/Practice5.h"
#include "Basic/Practice6/Practice6.h"
#include "Basic/Practice7/Practice7.h"

#include "Lighting/Color/Color.h"
#include "Lighting/BasicLighting/BasicLighting.h"
#include "Lighting/MaterialPractice/MaterialPractice.h"
#include "Lighting/LightingMap/LightingMap.h"
#include "Lighting/DirectionalLight/DirectionalLight.h"
#include "Lighting/PointLight/PointLight.h"
#include "Lighting/Spotlight/Spotlight.h"
#include "Lighting/MultipleLights/MultipleLights.h"

#include "Advanced_OpenGL/DepthTest/DepthTest.h"
