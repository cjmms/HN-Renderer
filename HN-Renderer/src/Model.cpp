#include "pch.h"
#include "Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace HN
{
	Model::Model(Device& device, const Builder& builder)
		:device{ device }
	{
		createVertexBuffers(builder.vertices);
		createIndexBuffers(builder.indices);
	}

	



	Model::~Model()
	{
		// clean vertices
		vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
		vkFreeMemory(device.device(), vertexBufferMemory, nullptr);

		if (hasIndexBuffer)
		{
			// clean indices
			vkDestroyBuffer(device.device(), indexBuffer, nullptr);
			vkFreeMemory(device.device(), indexBufferMemory, nullptr);
		}
	}


	void Model::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };		// vertex data starts from beginning, offset is 0 

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	 


	void Model::Draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexBuffer)
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		else
			vkCmdDraw(commandBuffer, vertexCount, vertexCount / 3, 0, 0);
	}


	void Model::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size()); 

		// Add an assertion that vertexCount is at least 3, because a triangle needs at lease 3 vertices to render
		assert(vertexCount >= 3 && "Vertex count must be at least 3");

		//VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;

		// staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: we want allocated mem be accessable to CPU. That way, CPU can write to GPU mem
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: keeps CPU mem consistent with GPU mem. 
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);


		void* hostData;
		// maps CPU mem(data) to GPU mem(vertexBufferMemory)
		vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &hostData);

		// copy vertex data into CPU mem
		memcpy(hostData, vertices.data(), static_cast<size_t>(bufferSize));

		// since GPU mem is mapped to CPU mem and
		// GPU mem is coherent to CPU mem
		// when vertices.data() get copied into hostData
		// the same data also get flushed to GPU mem automatically

		// unmap memory
		vkUnmapMemory(device.device(), stagingBufferMemory);



		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: we want allocated mem be accessable to CPU. That way, CPU can write to GPU mem
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: keeps CPU mem consistent with GPU mem. 
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,	
			vertexBuffer, vertexBufferMemory);

		// copy staging buffer to vertex buffer
		device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		// destroy staging buffer
		vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
		vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
	}



	void Model::createIndexBuffers(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0;

		if (!hasIndexBuffer) return; // no indices, no need to create index buffer

		VkDeviceSize bufferSize = sizeof(uint32_t) * indexCount;

		// staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: we want allocated mem be accessable to CPU. That way, CPU can write to GPU mem
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: keeps CPU mem consistent with GPU mem. 
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);


		void* hostData;
		// maps CPU mem(data) to GPU mem(vertexBufferMemory)
		vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &hostData);

		// copy vertex data into CPU mem
		memcpy(hostData, indices.data(), static_cast<size_t>(bufferSize));

		// since GPU mem is mapped to CPU mem and
		// GPU mem is coherent to CPU mem
		// when vertices.data() get copied into hostData
		// the same data also get flushed to GPU mem automatically

		// unmap memory
		vkUnmapMemory(device.device(), stagingBufferMemory);


		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: we want allocated mem be accessable to CPU. That way, CPU can write to GPU mem
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: keeps CPU mem consistent with GPU mem. 
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer, indexBufferMemory);

		// copy staging buffer to vertex buffer
		device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		// destroy staging buffer
		vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
		vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
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


	// 1. loader model
	// 2. create model obj ( unique ptr )
	std::unique_ptr<Model> Model::CreateModelFromFile(Device& device, const std::string& filepath)
	{
		Builder builder{};
		builder.loadModel(filepath);

		std::cout << "Model loaded: " << filepath << std::endl;
		std::cout << "Model vertices count: " << builder.vertices.size() << std::endl;

		return std::make_unique<Model>(device, builder);
	}


	void Model::Builder::loadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warning;
		std::string error;

		// loading
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filepath.c_str()))
			throw std::runtime_error(warning + error);
		

		vertices.clear();
		indices.clear();

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				// position
				if (index.vertex_index >= 0)
				{
					vertex.position = { 
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2] 
					};
				}

				// normal
				if (index.vertex_index >= 0)
				{
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				// texCoord
				if (index.vertex_index >= 0)
				{
					vertex.texCoords = {
						attrib.texcoords[3 * index.texcoord_index + 0],
						attrib.texcoords[3 * index.texcoord_index + 1],
					};
				}

				vertices.push_back(vertex);
			}
		}
	}
}

