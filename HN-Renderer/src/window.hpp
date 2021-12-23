#pragma once
#include "pch.h"

namespace HN
{
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, std::string name);

		// destroy window
		// terminate glfw
		~Window();

		Window(const Window&) = delete;
		void operator=(const Window&) = delete;

		inline bool ShouldClose() { return glfwWindowShouldClose(pWindow);  }

		inline VkExtent2D getExtent() { return{ static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(Height) }; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

	private:
		// init GLFW library
		void InitWindow();


	private:
		GLFWwindow* pWindow;

		const unsigned int Width;
		const unsigned int Height;

		std::string WindowName;
	};


}