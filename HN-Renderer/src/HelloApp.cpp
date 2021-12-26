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
        CreatePipeline();
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
            
            if (auto cmdBuffer = renderer.BeginFrame())
            {
                renderer.BeginSwapChainRenderPass(cmdBuffer);

                RenderGameObjs(cmdBuffer);

                renderer.EndSwapChainRenderPass(cmdBuffer);
                renderer.EndFrame();
            }
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
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderer.GetSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipeline = std::make_unique<Pipeline>(
            Device,
            "src/Shaders/simple_shader.vert.spv",
            "src/Shaders/simple_shader.frag.spv",
            pipelineConfig);
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
        transform.scale = { 2.0, 0.6 };

        // put all data together
        auto triangle = GameObj::CreateGameObject();
        triangle.model = model;
        triangle.color = {0.1f, 0.8f, 0.1f};
        triangle.transform2d = transform;
        

        gameObjs.push_back(std::move(triangle));      
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
            pushConstant.transform = obj.transform2d.mat2();

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