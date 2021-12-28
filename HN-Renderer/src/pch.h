// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

// add headers that you want to pre-compile here

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// make sure in all systems
// the glm will always expect angle input in radians
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE		// glm expects depth buffer range from [0, 1], rather than [-1, 1]
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstring>
#include <set>
#include <unordered_set>
#include <array>
#include <cstring>
#include <limits>
#include <vector>
#include <algorithm>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <cassert>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <stdint.h>
#include <fstream>
#include <memory>






const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;