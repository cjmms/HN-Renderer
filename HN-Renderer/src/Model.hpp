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
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal;
			glm::vec2 texCoords;

			static std::vector<VkVertexInputBindingDescription> getBindingDesciptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDesciptions();
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

	public:
		Model(Device& device, const Builder& builder);

		// destroy buffer, free memory 
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
	
		static std::unique_ptr<Model> CreateModelFromFile(Device &device, const std::string& filepath);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		Device& device;

		// In vulkan memory managment, buffer and memory are 2 seperate objects
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;
	};
}