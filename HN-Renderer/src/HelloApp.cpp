#include "pch.h"
#include "HelloApp.h"

namespace HN
{
    HelloTriangleApplication::HelloTriangleApplication()
    {
        LoadModel();
        CreatePipelineLayout();
        CreatePipeline();
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
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Pipeline layout.");
    }


    void HelloTriangleApplication::CreatePipeline()
    {
        auto pipelineConfig = Pipeline::DefaultPipelineConfigInfo(SwapChain.width(), SwapChain.height());
        pipelineConfig.renderPass = SwapChain.getRenderPass();
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
        commandBuffers.resize(SwapChain.imageCount());

        // allocate command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(Device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to create command buffers.");

        // record draw command to each buffer
        for (unsigned int i = 0; i < commandBuffers.size(); ++i)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("Command Buffer failed to begin recording.");

            // begin a render pass
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = SwapChain.getRenderPass();
            renderPassInfo.framebuffer = SwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = SwapChain.getSwapChainExtent();

            // set clear value
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline->Bind(commandBuffers[i]);
            model->Bind(commandBuffers[i]);
            model->Draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("Command Buffer failed to end recording.");
        }
    }


    void HelloTriangleApplication::DrawFrame()
    {
        uint32_t imageIndex;
        auto result = SwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire swap chain image");

        result = SwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);  // submit command buffer

        if (result != VK_SUCCESS) throw std::runtime_error("Failed to present swap chain image");
    }


    void HelloTriangleApplication::LoadModel()
    {
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        model = std::make_unique<Model>(Device, vertices);
    }
}