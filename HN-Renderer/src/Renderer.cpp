#include "pch.h"
#include "Renderer.hpp"


namespace HN
{
    Renderer::Renderer(Device& device, Window& window)
        :device{ device }, window{window}
    {
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    Renderer::~Renderer()
    {
        // since application may continue when renderer is destroyed
        // renderer has responsibility to free command buffers
        FreeCommandBuffers();
    }


    void Renderer::CreateCommandBuffers()
    {
        // at least 1 command buffer maps to 1 frambuffer
        cmdBuffers.resize(swapChain->imageCount());

        // allocate command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, cmdBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to create command buffers.");
    }




    void Renderer::RecreateSwapChain()
    {
        auto extent = window.getExtent();   // get current window size

        // if the window is minimized, GLFW will pause
        while (extent.width == 0 || extent.height == 0)
        {
            extent = window.getExtent();   // get current window size
            glfwWaitEvents();
        }

        // make sure current swapchain is not being used, otherwise, wait
        vkDeviceWaitIdle(device.device());

        if (swapChain == nullptr)
        {
            // since current swapchain is no longer being used, create the new swapchain
            swapChain = std::make_unique<SwapChain>(device, extent);
        }
        else
        {
            swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));

            if (swapChain->imageCount() != cmdBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

         // pipeline need to be recreated since it depends on swapchain
    }


    void Renderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(
            device.device(),
            device.getCommandPool(),
            static_cast<uint32_t>(cmdBuffers.size()),
            cmdBuffers.data());

        cmdBuffers.clear();
    }




	VkCommandBuffer Renderer::GetCurrentCmdBuffer() const 
	{
		assert(IsFrameInProgress && "Cannot get command buffer when frame not in progress");
		return cmdBuffers[currImgIndex]; 
	}



    // 1. acquire frame from swapchain
    // 2. get current cmd buffer
    // 3. begin cmd buffer recording
    VkCommandBuffer Renderer::BeginFrame()
    {
        assert(!IsFrameStarted && "Can't begin a frame when the frame is in progress.");

        // 1. acquire frame from swapchain
        auto result = swapChain->acquireNextImage(&currImgIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return nullptr;     // the frame has not successfully started
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw std::runtime_error("Failed to acquire swap chain image");
        isFrameStarted = true;

        // 2. get current cmd buffer
        auto cmdBuffer = GetCurrentCmdBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // 3. begin cmd buffer recording
        if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS) throw std::runtime_error("Command Buffer failed to begin recording.");

        return cmdBuffer;
    }


    // 1. get current cmd buffer
    // 2. end cmd buffer recording
    // 3. submit cmd buffer
    void Renderer::EndFrame()
    {
        assert(IsFrameStarted && "Can't end a frame when the frame is not in progress.");

        // 1. get current cmd buffer
        auto cmdBuffer = GetCurrentCmdBuffer();

        // 2. end cmd buffer recording
        if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS)
            throw std::runtime_error("Command Buffer failed to end recording.");

        // 3. submit cmd buffer
        auto result = swapChain->submitCommandBuffers(&cmdBuffer, &currImgIndex);  // submit command buffer

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
        {
            window.ResetWindowResizedFlag();
            RecreateSwapChain();
        }

        if (result != VK_SUCCESS) throw std::runtime_error("Failed to present swap chain image");

        isFrameStarted = false; // frame ends
    }


    // 1. begin render pass
    // 2. push viewport and scissor size change into cmd buffer
    void Renderer::BeginSwapChainRenderPass(VkCommandBuffer cmdBuffer)
    {
        assert(!IsFrameStarted && "Can't begin a render pass when the frame is in progress.");

        assert(cmdBuffer == GetCurrentCmdBuffer() && "Can't begin render pass on command buffer from different frame (cmd buffer does not match)");

        // render pass info
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(currImgIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        // set clear value
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        // 1. begin render pass
        vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // 2. push viewport and scissor size change into cmd buffer
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
        vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
    }



    void Renderer::EndSwapChainRenderPass(VkCommandBuffer cmdBuffer)
    {
        assert(IsFrameStarted && "Can't end a render pass when the frame is not in progress.");

        assert(cmdBuffer == GetCurrentCmdBuffer() && "Can't end render pass on command buffer from different frame (cmd buffer does not match)");

        vkCmdEndRenderPass(cmdBuffer);
    }


}