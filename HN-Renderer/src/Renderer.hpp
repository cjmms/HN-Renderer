#pragma once


#include "pch.h"
#include "window.hpp"
#include "SwapChain.hpp"


namespace HN
{
    class Renderer
    {
    public:
        Renderer(Device& device, Window& window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        // The renderer should be able to begin a frame
        // and record instructions into command buffer
        // and end the frame
        VkCommandBuffer BeginFrame();
        void EndFrame();

        // also begin render pass and end render pass
        void BeginSwapChainRenderPass(VkCommandBuffer cmdBuffer);
        void EndSwapChainRenderPass(VkCommandBuffer cmdBuffer);

        // Getter functions
        VkCommandBuffer GetCurrentCmdBuffer() const;
        inline bool IsFrameInProgress() const { return isFrameStarted; }
        inline VkRenderPass GetSwapChainRenderPass() const { return swapChain->getRenderPass(); }
        inline int GetFrameIndex() const { return currFrameIndex; }

    private:
        void CreateCommandBuffers();
        void FreeCommandBuffers();
        void RecreateSwapChain();

    private:
        Window &window;
        Device &device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> cmdBuffers;

        uint32_t currImgIndex;
        int currFrameIndex;     // used to keep track of frame index : [0, MAX_FRAMES_IN_FLIGHT]
        bool isFrameStarted = false;
    };
}