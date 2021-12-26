#include "pch.h"
#include "HelloApp.h"

namespace HN
{
    struct PushConstantData
    {
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    HelloTriangleApplication::HelloTriangleApplication()
    {
        LoadGameObjs();
        CreatePipelineLayout();
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    HelloTriangleApplication::~HelloTriangleApplication()
    {
        vkDestroyPipelineLayout(Device.device(), pipelineLayout, nullptr);
    }

    void HelloTriangleApplication::run()
    {
        while (!Window.ShouldClose())
        {
            glfwPollEvents();
            DrawFrame();
        }
    }

    void HelloTriangleApplication::CreatePipelineLayout()
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

        if (vkCreatePipelineLayout(Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Pipeline layout.");
    }


    void HelloTriangleApplication::CreatePipeline()
    {
        assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = swapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipeline = std::make_unique<Pipeline>(
            Device,
            "src/Shaders/simple_shader.vert.spv",
            "src/Shaders/simple_shader.frag.spv",
            pipelineConfig);
    }


    void HelloTriangleApplication::CreateCommandBuffers()
    {
        // at least 1 command buffer maps to 1 frambuffer
        commandBuffers.resize(swapChain->imageCount());

        // allocate command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(Device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to create command buffers.");
    }


    void HelloTriangleApplication::DrawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw std::runtime_error("Failed to acquire swap chain image");

        RecordCommandBuffer(imageIndex);
        result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);  // submit command buffer

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window.wasWindowResized())
        {
            Window.ResetWindowResizedFlag();
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS) throw std::runtime_error("Failed to present swap chain image");
    }


    void HelloTriangleApplication::LoadGameObjs()
    {
        // model data
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        auto model = std::make_shared<Model>(Device, vertices);

        // transform data
        Transform2dComponent transform{};
        transform.translation = {0.2f, 0.0f};

        // put all data together
        auto triangle = GameObj::CreateGameObject();
        triangle.model = model;
        triangle.color = {0.1f, 0.8f, 0.1f};
        triangle.transform2d = transform;

        gameObjs.push_back(std::move(triangle));      
    }



    void HelloTriangleApplication::RecreateSwapChain()
    {
        auto extent = Window.getExtent();   // get current window size

        // if the window is minimized, GLFW will pause
        while (extent.width == 0 || extent.height == 0)
        {
            extent = Window.getExtent();   // get current window size
            glfwWaitEvents();
        }

        // make sure current swapchain is not being used, otherwise, wait
        vkDeviceWaitIdle(Device.device());

        if (swapChain == nullptr)
        {
            // since current swapchain is no longer being used, create the new swapchain
            swapChain = std::make_unique<SwapChain>(Device, extent);
        }
        else
        {
            swapChain = std::make_unique<SwapChain>(Device, extent, std::move(swapChain));

            if (swapChain->imageCount() != commandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

        CreatePipeline(); // pipeline need to be recreated since it depends on swapchain
    }


    void HelloTriangleApplication::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(
            Device.device(), 
            Device.getCommandPool(), 
            static_cast<uint32_t>(commandBuffers.size()), 
            commandBuffers.data());

        commandBuffers.clear();
    }


    void HelloTriangleApplication::RecordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Command Buffer failed to begin recording.");

        // begin a render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        // set clear value
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // record new viewport and scissor
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        // render
        RenderGameObjs(commandBuffers[imageIndex]);

        // end render pass
        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("Command Buffer failed to end recording.");
    }





    void HelloTriangleApplication::RenderGameObjs(VkCommandBuffer cmdBuffer)
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