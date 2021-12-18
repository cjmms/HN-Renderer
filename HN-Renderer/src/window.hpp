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

		inline bool ShouldClose() { return glfwWindowShouldClose(pWindow);  }

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