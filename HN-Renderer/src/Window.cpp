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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		pWindow = glfwCreateWindow(Width, Height, WindowName.c_str(), nullptr, nullptr);
	}




}