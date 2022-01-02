#pragma once

#include "pch.h"
#include "Device.hpp"
#include "Pipeline.hpp"
#include "GameObj.hpp"
#include "FrameInfo.hpp"

namespace HN
{

    class RenderSystem
    {
    public:
        RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();


        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;

        void RenderGameObjs(FrameInfo &frameInfo, std::vector<GameObj>& gameObjs);

    private:

        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipeline(VkRenderPass renderPass);


        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}