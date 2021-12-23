#include "pch.h"

#include "Pipeline.hpp"


namespace HN {

	Pipeline::Pipeline(
		Device& device, 
		const std::string& vertFilePath, 
		const std::string& fragFilePath, 
		const PipelineConfigInfo& configInfo)
		: device{device}
	{
		createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
	}

	Pipeline::~Pipeline(){}

	std::vector<char> Pipeline::readFile(const std::string& filePath)
	{
		// read file in binary to avoid unexpected text translation
		std::ifstream file{filePath, std::ios::ate | std::ios::binary};

		// check if file is open
		if (!file.is_open()) throw std::runtime_error("Failed to open file(check if address is correct): " + filePath);

		// file.tellg() returns last position of file
		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		// move to the beginning of the file
		file.seekg(0);

		// read file into buffer
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}


	void Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
		std::cout << "Frag Shader Code Size: " << fragCode.size() << std::endl;
	}


	PipelineConfigInfo Pipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo{};

		return configInfo;
	}


	void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();

		// Vulkan expects compiled shader info as uint32_t type
		// this case is valid because code is a vector, so that alignment satisfies the requirment
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());	

		if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader module.");
	}
}