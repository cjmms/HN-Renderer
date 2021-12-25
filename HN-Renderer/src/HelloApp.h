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
        HelloTriangleApplication& operator=(const HelloTriangleApplication&) = delete;

    private:
        void LoadModel();
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
        void FreeCommandBuffers();
        void DrawFrame();
        void RecreateSwapChain();
        void RecordCommandBuffer(int imageIndex);

        Window Window{WIDTH, HEIGHT, "hello world"};
        Device Device{ Window };
        std::unique_ptr<SwapChain> swapChain;

        std::unique_ptr<Pipeline> pipeline;

        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
    
    };

}