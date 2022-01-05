#include "pch.h"

#include "RenderSystem.hpp"



namespace HN
{
    struct PushConstantData
    {
        glm::mat4 modelMat{ 1.0f };
    };


    RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : device{device}
    {
        CreatePipelineLayout(globalSetLayout);
        CreatePipeline(renderPass);
    }


    RenderSystem::~RenderSystem()
    {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }



    void RenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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
            "src/Shaders/simple_shader.geom.spv",
            "src/Shaders/simple_shader.frag.spv",
            pipelineConfig);
    }




    void RenderSystem::RenderGameObjs(FrameInfo &frameInfo, std::vector<GameObj>& gameObjs)
    {
        // bind pipeline
        pipeline->Bind(frameInfo.cmdBuffer);

        // bind descriptor to the pipeline
        vkCmdBindDescriptorSets(
            frameInfo.cmdBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0, 1, &frameInfo.globalDescriptorSet,
            0, nullptr
        );

        
        // 1. push constants
        // 2. bind model
        // 3. draw
        for (auto& obj : gameObjs)
        {
            PushConstantData pushConstant{};

            pushConstant.modelMat = obj.transform.mat4();   // model matrix

            vkCmdPushConstants(
                frameInfo.cmdBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(PushConstantData),
                &pushConstant);

            obj.model->Bind(frameInfo.cmdBuffer);
            obj.model->Draw(frameInfo.cmdBuffer);
        }
    }
}