#pragma once

#include "pch.h"
#include "Device.hpp"
#include "Pipeline.hpp"
#include "GameObj.hpp"
#include "Camera.hpp"


namespace HN
{

    class RenderSystem
    {
    public:
        RenderSystem(Device& device, VkRenderPass renderPass);
        ~RenderSystem();


        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;

        void RenderGameObjs(VkCommandBuffer commandBuffer, std::vector<GameObj>& gameObjs, const Camera& camera);

    private:

        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);


        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}