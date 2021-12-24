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

	Pipeline::~Pipeline()
	{
		vkDestroyShaderModule(device.device(), vertShaderModule, nullptr);	// vertex shader module
		vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);	// fragment shader module
		vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
	}

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

		createShaderModule(vertCode, &vertShaderModule);
		createShaderModule(fragCode, &fragShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShaderModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;


		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShaderModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		auto bindingDescriptions = Model::Vertex::getBindingDesciptions();
		auto attribDescriptions = Model::Vertex::getAttributeDesciptions();
		// determines how to interperate vertex buffer data
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribDescriptions.size());	// no attribute descriptor, hard code vertex info
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputInfo.pVertexAttributeDescriptions = attribDescriptions.data();

		VkPipelineViewportStateCreateInfo viewportInfo{};
		// Viewport Info (used to create viewport and scissor)
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &configInfo.viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &configInfo.scissor;

		VkGraphicsPipelineCreateInfo pipelineInfo{};

		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
		pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = configInfo.pipelineLayout;
		pipelineInfo.renderPass = configInfo.renderPass;
		pipelineInfo.subpass = configInfo.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Graphics Pipeline.");
	}


	PipelineConfigInfo Pipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo{};

		// Input Assembly 
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		// viewport setup
		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;
		configInfo.viewport.width = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f;
		configInfo.viewport.maxDepth = 1.0f;

		// scissor setup
		configInfo.scissor.offset = { 0, 0 };
		configInfo.scissor.extent = { width, height };

		// Raster
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;	// Disabled: force depth between [0, 1]
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;	// draw edges? vertices? or filled in
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;			// No back face culling
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;	// clockwise as winding order
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		// Multisampling
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		// Blending
		configInfo.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		// depth / stencil
		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {};  // Optional
		configInfo.depthStencilInfo.back = {};   // Optional

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



	void Pipeline::Bind(VkCommandBuffer commandBuffer)
	{
		// There are 3 pipelines in vulkan
		// Graphics Pipeline
		// Compute Pipeline
		// RayTracing Pipeline
		// In this case, it's assumed command buffer always binds graphics pipeline
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}
}