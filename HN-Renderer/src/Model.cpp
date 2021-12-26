#include "pch.h"
#include "Model.hpp"


namespace HN
{
	Model::Model(Device& device, const std::vector<Vertex>& vertices)
		:device{ device }
	{
		createVertexBuffers(vertices);
	}

	



	Model::~Model()
	{
		vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
		vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
	}


	void Model::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };		// vertex data starts from beginning, offset is 0 

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}



	void Model::Draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, vertexCount / 3, 0, 0);
	}


	void Model::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size()); 

		// Add an assertion that vertexCount is at least 3, because a triangle needs at lease 3 vertices to render
		assert(vertexCount >= 3 && "Vertex count must be at least 3");

		//VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;

		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: we want allocated mem be accessable to CPU. That way, CPU can write to GPU mem
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: keeps CPU mem consistent with GPU mem. 
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,	
			vertexBuffer, vertexBufferMemory);

		void* hostData;
		// maps CPU mem(data) to GPU mem(vertexBufferMemory)
		vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &hostData);

		// copy vertex data into CPU mem
		memcpy(hostData, vertices.data(), static_cast<size_t>(bufferSize));

		// since GPU mem is mapped to CPU mem and
		// GPU mem is coherent to CPU mem
		// when vertices.data() get copied into hostData
		// the same data also get flushed to GPU mem automatically

		// unmap memory
		vkUnmapMemory(device.device(), vertexBufferMemory);
	}




	// vertex struct

	std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDesciptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDesc(1);
		bindingDesc[0].binding = 0;
		bindingDesc[0].stride = sizeof(Vertex);
		bindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDesc;
	}


	std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDesciptions()
	{
		std::vector<VkVertexInputAttributeDescription> attribDesc(2);
		attribDesc[0].binding = 0;
		attribDesc[0].location = 0;							// location of position
		attribDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;		// three floats
		attribDesc[0].offset = 0;

		attribDesc[1].binding = 0;
		attribDesc[1].location = 1;
		attribDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDesc[1].offset = offsetof(Vertex, color);

		return attribDesc;
	}
}

