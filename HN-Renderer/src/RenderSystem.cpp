#include "pch.h"

#include "RenderSystem.hpp"



namespace HN
{
    struct PushConstantData
    {
        glm::mat2 transform{ 1.0f };
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };


    RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass) : device{device}
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
    }


    RenderSystem::~RenderSystem()
    {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }



    void RenderSystem::CreatePipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Pipeline layout.");
    }


    void RenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipeline = std::make_unique<Pipeline>(
            device,
            "src/Shaders/simple_shader.vert.spv",
            "src/Shaders/simple_shader.frag.spv",
            pipelineConfig);
    }




    void RenderSystem::RenderGameObjs(VkCommandBuffer cmdBuffer, std::vector<GameObj>& gameObjs)
    {
        // bind pipeline
        pipeline->Bind(cmdBuffer);

        // 1. push constants
        // 2. bind model
        // 3. draw
        for (auto& obj : gameObjs)
        {
            PushConstantData pushConstant{};
            pushConstant.offset = obj.transform2d.translation;
            pushConstant.color = obj.color;
            pushConstant.transform = obj.transform2d.mat2();

            vkCmdPushConstants(
                cmdBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(PushConstantData),
                &pushConstant);

            obj.model->Bind(cmdBuffer);
            obj.model->Draw(cmdBuffer);
        }
    }
}