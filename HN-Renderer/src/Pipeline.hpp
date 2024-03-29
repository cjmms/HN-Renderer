#pragma once
#include "pch.h"
#include "Device.hpp"
#include "Model.hpp"

namespace HN {

	struct PipelineConfigInfo {
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;

		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};


	class Pipeline
	{
	public:
		Pipeline(
			Device& device, 
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo& configInfo);

		Pipeline(
			Device& device,
			const std::string& vertFilePath,
			const std::string& tescFilePath,
			const std::string& teseFilePath,
			const std::string& geomFilePath,
			const std::string& fragFilePath,
			const PipelineConfigInfo& configInfo);

		~Pipeline();

		Pipeline(const Pipeline&) = delete;	// delete copy constructor
		Pipeline& operator=(const Pipeline&) = delete;

		void Bind(VkCommandBuffer commandBuffer);

		// Input Assembler:
		// Triangle List
		// ViewPort:
		// using width, height. Depth[0, 1]
		// Scissor: 
		// Render the whole viewport
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo& configInfo);

		void createGraphicsPipeline(
			const std::string& vertFilePath,
			const std::string& tescFilePath,
			const std::string& teseFilePath,
			const std::string& geomFilePath,
			const std::string& fragFilePath,
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		// 1. load shader
		// 2. create shader module
		// 3. create shader stage create info
		VkPipelineShaderStageCreateInfo LoadShader(const std::string& shaderFilePath, VkShaderStageFlagBits shaderStage);

		Device& device;
		VkPipeline graphicsPipeline;
		std::vector<VkShaderModule> shaderModules;
	};

	
}