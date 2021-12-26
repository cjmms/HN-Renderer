#pragma once

#include "pch.h"
#include "window.hpp"
#include "Pipeline.hpp"
#include "SwapChain.hpp"
#include "GameObj.hpp"
#include "Renderer.hpp"


namespace HN
{
   



    class HelloTriangleApplication
    {
    public:
        HelloTriangleApplication();
        ~HelloTriangleApplication();
        void run();

        HelloTriangleApplication(const HelloTriangleApplication&) = delete;
        HelloTriangleApplication& operator=(const HelloTriangleApplication&) = delete;

    private:
        void LoadGameObjs();
        void CreatePipelineLayout();
        void CreatePipeline();


        void RenderGameObjs(VkCommandBuffer commandBuffer);

        Window Window{WIDTH, HEIGHT, "hello world"};
        Device Device{ Window };

        Renderer renderer{ Device, Window };

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        std::vector<GameObj> gameObjs;
    };

}