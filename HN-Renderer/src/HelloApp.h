#pragma once

#include "pch.h"
#include "window.hpp"
#include "Pipeline.hpp"
#include "SwapChain.hpp"



namespace HN
{

    class HelloTriangleApplication
    {
    public:
        HelloTriangleApplication();
        ~HelloTriangleApplication();
        void run();

        HelloTriangleApplication(const HelloTriangleApplication&) = delete;
        void operator=(const HelloTriangleApplication&) = delete;

    private:
        void LoadModel();
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
        void DrawFrame();

        Window Window{WIDTH, HEIGHT, "hello world"};
        Device Device{ Window };
        SwapChain SwapChain{ Device, Window.getExtent() };

        std::unique_ptr<Pipeline> pipeline;

        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
    
    };

}