#pragma once

#include "pch.h"
#include "Device.hpp"


namespace HN
{
	// Purpose of this class
	// 1. Read input vertex information
	// 2. allocate vertex buffer memory
	// 3. copy vertex infomation into vertex buffer with specific layout
	class Model
	{
	public:
		// vertex struct, specifies what data each vertex contains
		struct Vertex
		{
			glm::vec2 position;

			static std::vector<VkVertexInputBindingDescription> getBindingDesciptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDesciptions();
		};

	public:
		Model(Device& device, const std::vector<Vertex>& vertices);

		// destroy buffer, free memory 
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);

		Device& device;

		// In vulkan memory managment, buffer and memory are 2 seperate objects
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		uint32_t vertexCount;
	};
}