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
		Window& operator=(const Window&) = delete;

		inline bool ShouldClose() { return glfwWindowShouldClose(pWindow);  }

		inline VkExtent2D getExtent() const { return{ static_cast<uint32_t>(Width), static_cast<uint32_t>(Height) }; }

		inline bool wasWindowResized() const  { return framebufferResized; }
		inline void ResetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
		inline GLFWwindow* GetWindow(){ return pWindow; }

	private:
		// init GLFW library
		void InitWindow();
		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);


	private:
		GLFWwindow* pWindow;

		unsigned int Width;
		unsigned int Height;
		bool framebufferResized = false; 

		std::string WindowName;
	};


}