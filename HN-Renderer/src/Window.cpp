#pragma once
#include "pch.h"
#include "window.hpp"


namespace HN
{
	Window::Window(unsigned int width, unsigned int height, std::string name)
		:Width{ width }, Height{ height }, WindowName{ name }
	{
		InitWindow();
	}


	Window::~Window()
	{
		glfwDestroyWindow(pWindow);
		glfwTerminate();
	}


	void Window::InitWindow()
	{
		glfwInit();

		// Telling glfw that context is not created for OpenGL
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// disable window resize
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		pWindow = glfwCreateWindow(Width, Height, WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(pWindow, this);
		// whenever the window is resized, the resize call back function will excute
		glfwSetFramebufferSizeCallback(pWindow, framebufferResizeCallback);
	}


	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, pWindow, nullptr, surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface.");
	}


	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto pWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		pWindow->framebufferResized = true;
		pWindow->Width = width;
		pWindow->Height = height;
	}

}